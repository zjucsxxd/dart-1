/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Implementation for creating,      *
 *                          - inserting and deleting from ordered arrays *
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
 
#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include "common.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  This array is insertion sorted - it always remains in a sorted state (between calls).*
 * It can store anything that can be cast to a void* -- so a u32int, or any pointer.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef void* type_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * A predicate should return nonzero if the first argument is less than the second. Else *
 * it should return zero.                                                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef s8int (*lessthan_predicate_t)(type_t,type_t);
typedef struct
{
    type_t *array;
    u32int size;
    u32int max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

/* * * * * * * * * * * * * * * * * * 
 * A standard less than predicate. *
 * * * * * * * * * * * * * * * * * */
s8int standard_lessthan_predicate(type_t a, type_t b);

/* * * * * * * * * * * * * * * * * * 
 * Create an ordered array.        *
 * * * * * * * * * * * * * * * * * */
ordered_array_t create_ordered_array(u32int max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, u32int max_size, lessthan_predicate_t less_than);

/* * * * * * * * * * * * * * * * * * 
 * Destroy an ordered array.       *
 * * * * * * * * * * * * * * * * * */
void destroy_ordered_array(ordered_array_t *array);

/* * * * * * * * * * * * * * * * * * 
 * Add an item into the array.     *
 * * * * * * * * * * * * * * * * * */
void insert_ordered_array(type_t item, ordered_array_t *array);

/* * * * * * * * * * * * * * * * * * 
 * Lookup the item at index i.     *
 * * * * * * * * * * * * * * * * * */
type_t lookup_ordered_array(u32int i, ordered_array_t *array);

/* * * * * * * * * * * * * * * * * * * * * * * * *
 * Deletes the item at location i from the array.*
 * * * * * * * * * * * * * * * * * * * * * * * * */
void remove_ordered_array(u32int i, ordered_array_t *array);

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                   End of file ordered_array.h                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
