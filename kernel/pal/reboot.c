#include "reboot.h"   // it obvious what this is
#include "stdio.h"    // printf function
#include "common.h"   // outb and inb
#include "timer.h"    // provides the number of ticks

void reboot()
{
    byte temp;
	printf ("\n");
    printf ("* Rebooting your computer in 3 seconds");
	ksleep(100);  // Sleep for 1 second
    
	// disable all interrupts 
    _asm 
	{
		cli
	}
	ksleep(100);  // Sleep for another 1 second
    // Clear all keyboard buffers (output and command buffers) 
    do
    {
	    // Even though the keyboard doesn't work yet, this
		// piece of code should work without any problem.
        temp = inb(KBRD_INTRFC);  // empty user data 
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
		{
            inb(KBRD_IO);         // empty keyboard data 
		}
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);
	ksleep(100);  // Sleep for 1 more second
 
    outb(KBRD_INTRFC, KBRD_RESET);    // pulse CPU reset line 
	// Show a fault message - I avoided the PANIC() method here for an obvious 
	// reason.
	printf ("KRNDBG: Something went wrong. Your computer will now be halted");
    // TODO: Go to kernel debugger in order to avoid halting the system 
	//       with the code bellow
    _asm 
	{
		hlt // Shouldn't be here, so halt the system
	}  
}

void ksleep (int ms)
{
	static int ticks = ms + tick;
	while (ticks > tick)
		;
	return;
}