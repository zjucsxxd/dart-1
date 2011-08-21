/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Loads and runs a *.PE file        *
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

#include "pe.h"
#include "fs.h"
#include "common.h"

// Loads the *.PE binary
int load_pe(u32int offset)
{
     PPEHeader_t header = (PPEHeader_t)offset;

     //if(header->e_magic != (unsigned short)"MZ")
     //     return 1;
          
     //PPEFileHeader_t fheader = (PPEFileHeader_t)(offset+header->e_lfanew);

     PPEOptionalHeader_t opheader = (PPEOptionalHeader_t)(offset+header->e_lfanew+24);
     
     execute_binary(opheader->AddressOfEntryPoint);
     return 0;
}

// Executes the *.PE when loaded
void execute_module_pe(char* name, u32int size)
{
     fs_node_t *modulenode = finddir_fs(fs_root, name);
     char modulebuffer[size];
     unsigned int modulesize = read_fs(modulenode, 0, size, modulebuffer);
     unsigned int moduleptr = kmalloc(modulesize);
     memcpy((void*)moduleptr,(void*) modulebuffer, modulesize);
     if(load_pe(moduleptr))
     {
		 kprintf("Error loading %s\n",name);
     }
}
