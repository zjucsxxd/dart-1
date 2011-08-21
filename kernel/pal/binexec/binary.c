/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Loads and runs a binary file      *
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

#include "fs.h"
#include "common.h"

void execute_binary(unsigned int addr)
{
     __asm__ __volatile__("call *%0" : : "a" (addr));
}

//Loads a flat binary into our system's memory and runs it
void execute_module_binary(char* name, u32int size)
{
     fs_node_t *modulenode = finddir_fs(fs_root, name);
     char* modulebuffer = (char*)kmalloc(size);
     unsigned int modulesize = read_fs(modulenode, 0, size, modulebuffer);
     unsigned int moduleptr = kmalloc(modulesize);
     memcpy((void*)moduleptr,(void*) modulebuffer, modulesize);
     execute_binary(moduleptr);
}
