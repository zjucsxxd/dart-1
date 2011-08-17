/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the kernel entry point    *
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

#include "screen.h"
#include "dt.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "isr.h"
// #include "keybd.h" if you add this you will get an error
// The above header is already included in isr.h

void shell();

extern u32int placement_address;
u32int initial_esp;

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *                Our main function              *
 * * * * * * * * * * * * * * * * * * * * * * * * */
int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    kclear();
    kprintf("Welcome to Codename Pegasus Operating System!\n");
    kprintf("*   Start of [kernel mode]\n");
    // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(100);
	kprintf("*   CMOS timer initialised to 100hz\n");
    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;
    
    kprintf("*   Initalizing paging");
    // Start paging.
    initialise_paging();
    kprintf("*   Installing keyboard driver");
	// Start keyboard driver - install it
    init_keyboard();
    // Start multitasking.
    initialise_tasking();
    kprintf("*   Enabling Interrupt Controler IC");
    __asm__ __volatile__ ("sti");
    // Initialise the initial ramdisk, and set it as the filesystem root.
    syscall_kprintf("*   Initializing initial ramdisk\n");
    syscall_kprintf("*   Setting the filesystem root\n");
    fs_root = initialise_initrd(initrd_location);
	
    syscall_kprintf("*   Initializing system calls\n");
    initialise_syscalls();
    syscall_kprintf("*   End of kernel mode\n");

    switch_to_user_mode();

    syscall_kprintf("*   Start of [ user mode ]\n");
    syscall_kprintf("Pegasus Has booted!\n");

    //start_task(shell,0); // Kernel panic situation fixed, does not read characters
    //shell();
    
    for(;;)
    {
       //endless loop
    }
    
    syscall_kprintf("\n\n\nreturning status 0\n\tsuccessful session\n");
    syscall_kprintf("\n\t[you can now power off your computer]\n");
    
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *            The console application            *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void shell()
{
    while(1)
    {
       syscall_kprintf("[pegasus@root]$ ");
       static char* cmd;
       cmd = gets();
       //unsigned char cmd;
       //cmd = getch();
       // Try adding it to a process list
       // Enable interrupts and then try again!
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                        End of file main.c                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
