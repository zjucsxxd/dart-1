/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - paging.h                          *
 * Copyright (C) 2011  Constantine M. Apostolou                          *
 * Author: Constantine M. Apostolou - conmarap@osarena.net               *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct page
{
    u32int present    : 1;   // Page present in memory
    u32int rw         : 1;   // Read-only if clear, readwrite if set
    u32int user       : 1;   // Supervisor level only if clear
    u32int accessed   : 1;   // Has the page been accessed since last refresh?
    u32int dirty      : 1;   // Has the page been written to since last refresh?
    u32int unused     : 7;   // Amalgamation of unused and reserved bits
    u32int frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Array of pointers to pagetables.
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    page_table_t *tables[1024];
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Array of pointers to the pagetables above, but gives their *physical*
     * location, for loading into the CR3 register.
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    u32int tablesPhysical[1024];

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * The physical address of tablesPhysical. This comes into play
     * when we get our kernel heap allocated and the directory
     * may be in a different location in virtual memory.
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    u32int physicalAddr;
} page_directory_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Sets up the environment, page directories etc and
 * enables paging.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void initialise_paging();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Causes the specified page directory to be loaded into the
 * CR3 register.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void switch_page_directory(page_directory_t *new);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Retrieves a pointer to the page required.                   
 * If make == 1, if the page-table in which this page should   
 * reside isn't created, create it!                            
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
page_t *get_page(u32int address, int make, page_directory_t *dir);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Handler for page faults.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void page_fault(registers_t *regs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Makes a copy of a page directory.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
page_directory_t *clone_directory(page_directory_t *src);

#endif
