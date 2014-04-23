/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - probes memory above 1MB of memory *
 * Copyright (C) 2011  Constantine M. Apostolou                          *
 * From OSDev wiki                                                       *
 * Modified by: Constantine M. Apostolou - conmarap@osarena.net          *
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
  
#include <common.h>         // Include the commonly used variable types  
#include <isr.h>            // Include interrupt table

void count_memory(void)
{
	// Register all the variables to be used
	register u32int *mem;
	u32int	mem_count, a;
	u16int	memkb;
	u8int	irq1, irq2;
	u32int	cr0;
 
	// Save the IRQ's
	IRQ1=inb(0x21);         // Don't know if this will work since the keyboard
	                        // is installed at this IRQ
	IRQ2=inb(0xA1);         // This will probably work fine
 
	// Kill all of the IRQ's
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
 
	mem_count=0;
	memkb=0;
 
	// Store a copy of CR0
	__asm__ __volatile("movl %%cr0, %%eax":"=a"(cr0))::"eax");
 
	// Invalidate the cache
	// Write-back and invalidate the cache
	__asm__ __volatile__ ("wbinvd");
 
	// plug cr0 with just PE/CD/NW
	// cache disable(486+), no-writeback(486+), 32bit mode(386+)
	__asm__ __volatile__("movl %%eax, %%cr0", ::
		"a" (cr0 | 0x00000001 | 0x40000000 | 0x20000000) : "eax");
 
	// Enter a loop to detect and count the memory of the system
	do {
		memkb++;
		mem_count += 1024*1024;
		mem= (u32int*) mem_count;
 
		a= *mem;
		*mem= 0x55AA55AA;
 
          // the empty asm calls tell gcc not to rely on what's in its registers
          // as saved variables (this avoids GCC optimisations)
		asm("":::"memory");
		if (*mem!=0x55AA55AA) mem_count=0;
		else {
			*mem=0xAA55AA55;
			asm("":::"memory");
			if(*mem!=0xAA55AA55)
			mem_count=0;
		}
 
		asm("":::"memory");
		*mem=a;
 
	} while (memkb<4096 && mem_count!=0);
 
	__asm__ __volatile__("movl %%eax, %%cr0", :: "a" (cr0) : "eax");
 
	mem_end = memkb<<20;
	mem = (u32int*) 0x413;
	bse_end= (*mem & 0xFFFF) <<6;
 
	// Re-write the IRQ's
	outb(0x21, IRQ1);
	outb(0xA1, IRQ2);
}