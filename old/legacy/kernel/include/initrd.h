/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the interface structures  *
 *                                   - relating to the initial ramdisk   *
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
#ifndef INITRD_H
#define INITRD_H

#include "common.h"
#include "fs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Typedef struct containing a var with the    *
 *         number of files in our ramdisk        *
 * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct
{
    u32int nfiles; 
} initrd_header_t;

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Typedef struct containing vars for error    *
 * checking, Filenames, the offset in the initrd *
 *that the file starts and the Length of the file*
 * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct
{
    u8int magic;     // Magic number, for error checking.
    s8int name[64];  // Filename.
    u32int offset;   // Offset in the initrd that the file starts.
    u32int length;   // Length of the file.
} initrd_file_header_t;

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *       Initialises the initial ramdisk.        *
 *   Returns a completed filesystem node after   *
 *getting passed the address of the multiboot mod*
 * * * * * * * * * * * * * * * * * * * * * * * * */
fs_node_t *initialise_initrd(u32int location);

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       End of file initrd.h                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
