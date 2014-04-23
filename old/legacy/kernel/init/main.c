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

//NOTES: There is some kind of problem with paging.c, check it out and 
//       fix the f@cking problem that is f@cking pissing me off with all
//       those f@cking paging faults -> October 31 2011

#include "screen.h"          // Contains the kprintf, kput and kclear
#include "dt.h"              // Contains the global descriptor table
#include "timer.h"           // Contains the cmos timer interface
#include "paging.h"          // Contains the interface for paging
#include "multiboot.h"       // Contains the multiboot interface
#include "fs.h"              // Contains the file system interface
#include "initrd.h"          // Contains the initrd interface
#include "task.h"            // Contains the multitasking interface
#include "syscall.h"         // Contains the system call interface
#include "cpuid.h"           // Contains the interface for retreiving cpu info
#include "stdio.h"           // Contains the stdio functions 
#include "keybd.h"           // Contains the keyboard interface
#include "pci.h"             // Contains the pci interface
#include "rtc.h"             // Contains the interface for returning the system time

void shell();                // Define the shell function

extern u32int placement_address;
u32int initial_esp;

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *                Our main function              *
 * * * * * * * * * * * * * * * * * * * * * * * * */
int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    // ------------------------------------------------------------------------------------------
    //                               Start of Kernel Mode
    // ------------------------------------------------------------------------------------------
    // Main entry to the kernel
    initial_esp = initial_stack;

    // Initialise all the ISRs and segmentation
    init_descriptor_tables();

    // Initialise the screen by clearing it
    kclear();
    kprintf("Welcome to Codename Pegasus Operating System\n\n");
    kprintf("* Start of kernel mode\n");
    kprintf("*   Now loading kernel modules\n");

    // Initialize interrupts	
    kprintf("*   Initializing Interrupts\n");
    __asm__ __volatile__ ("sti");

    // Initialise the PIT to 100Hz
    init_timer(50);
    kprintf("*   CMOS timer initialised to 100hz\n");

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);

    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    kprintf("*   Initalizing paging\n");
    // Start paging.
    initialise_paging();

    // Preform memory allocation test
    kprintf("*   Preforming MAM test (malloc)\n");
    mmutst();

    kprintf("*   Initializing multitasking\n");

    // Start multitasking.
    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    kprintf("*   Initializing initial ramdisk\n");
    kprintf("*   Setting the filesystem root\n");
    fs_root = initialise_initrd(initrd_location);

    // Start keyboard driver - install it
    kprintf("*   Installing keyboard driver\n");
    keyboard_install();

    kprintf("*   Initializing system calls\n");
    initialise_syscalls();
    kprintf("*   End of kernel mode\n");

    // This is mainly a way to test the printf function
    char *str = "PAL";
    printf("*   Pegasus Aplication Layer - %s\n", str);

    // ------------------------------------------------------------------------------------------
    //                               Switch to User Mode
    // ------------------------------------------------------------------------------------------
    // For the love of God... DO NOT use kernel functions in User Mode, ONLY SYSCALLS!
    // ------------------------------------------------------------------------------------------
    //switch_to_user_mode(); // This returns a fucking paging fault - kernel panic 
                             // TODO: For some freaking reason, switching to usermode returns a 
                             //       kernel panic, check out all the functions related to 
                             //       syscalls and whatever is inside the virtual user land
                             //       The kernel panic occures right after, or during the process
                             //       of entering User Mode. Specific message:
                             //       
                             //       Page fault! ( present user-mode ) at 0x0x19001213 - EIP: 0x120093 
                             //       PANIC! [Page fault] -> paging.c:246
    // ------------------------------------------------------------------------------------------    
    printf("* Start of user mode\n");

    init_pci();
    detect_floppy_drives();
    detectVideoType();
    printf("*   System RAM->");
    kprintf_dec(ramDetect()); 
    printf(" Bytes\n");
    printf("\nPegasus has booted successfully\n\n");

    //pclear(); // Well functioning (pclear() is used ONLY in user mode)
                //                  (kclear() is ONLY for kernel mode  )
    //printf("*   Random number->%d \n",random(0,10000));

    printf("*   Entering infinite loop\n");
  
    // printf() works in both kernel and user mode
    // Though its mostly aimed to be used in user mode
    for(;;)
    {
       //endless loop
    }
    
    printf("\n\n\n*   returning status 0\n\tsuccessful session\n");
    printf("\n\t[you can now power off your computer]\n");
    
    return 0;
    
    // -------------------------------------------------------------------------------------------
    // Functions that can be used 
    //
    //    init_pci();     - Detects the devices on our current computer   [user mode][           ]
    //    detect_cpu();   - Detects the model of the CPU on our computer  [user mode][           ]
    //    init_rtc();     - Detects and returns the current system time   [user mode][kernel mode]
    //    detect_floppy_drives();
    //                    - Detects the installed floppy drives           [user mode][kernel mode]
    //    ramDetect();    - Detects the system's RAM                      [user mode][kernel mode]
    //    detectVideoType();
    //                    - Detects the type of the monitor (mono/color)  [user mode][           ]
    //    random(u32int,u32int);
    //                    - Creates a random number and returns it        [user mode][kernel mode]
    //
    // -------------------------------------------------------------------------------------------
    //                                  End of kernel
    // -------------------------------------------------------------------------------------------
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *            The console application            *
 * * * * * * * * * * * * * * * * * * * * * * * * */
///TODO: Check function "gets()" and if interrupts are enabled
void shell()
{
    for(;;)
    {
       printf("[pegasus]-> ");
       //static char* cmd;
       //cmd = getch();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *              Startup malloc test              *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void mmutst();
void mmutst()
{
    u32int malloc_test = kmalloc(100);
    if(malloc_test != 0)
    {
         kprintf("    MAM working well: ");
         kprintf_hex(malloc_test);
         kprintf(" Bytes allocated\n");
         kprintf("    Now freeing allocated memory\n");
         kfree((void*)malloc_test);
    }
    else
    {
         kprintf("    MAM test failed...\n");    
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                        End of file main.c                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ------------------------------------------------------------------------
// CODE TO TEST
// ------------------------------ Fork test -------------------------------
/*    // Create a new process in a new address space which is a clone of this.
    int ret = fork();
    printf("fork() returned %i", ret);
    printf(", and getpid() returned %i", getpid());
    printf("\n============================================================================\n");

    asm volatile("cli");
    // The next section of code is not reentrant so make sure we aren't interrupted during.
    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        printf("Found file ");
        printf(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);
        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            printf("\n\t(directory)\n");
        }
        else
        {
            printf("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                kput(buf[j]);
            printf("\"\n");
        }
        i++;
    }
    printf("\n");

    asm volatile("sti"); */
// ------------------------- Fork test -------------------------


