/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - High level interrupt service      *
 *         - routines and interrupt request handlers. Part of this code  *
 *         - is modified from Bran's kernel development tutorials.       *
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

#include "common.h"
#include "isr.h"
#include "screen.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *     This gets called from our ASM interrupt   *
 *                   handler stub.               *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void isr_handler(registers_t regs)
{
    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
    u8int int_no = regs.int_no & 0xFF;
    if (interrupt_handlers[int_no] != 0)
    {
        isr_t handler = interrupt_handlers[int_no];
        handler(&regs);
    }
    else
    {
        kprintf("unhandled interrupt: ");
        kprintf_hex(int_no);
        kput('\n');
        for(;;);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *Gets called from our ASM interrupt handler stub*
 * * * * * * * * * * * * * * * * * * * * * * * * */
void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.

    // This is a blank function pointer 
    void (*handler)(registers_t regs);

    // Find out if we have a custom handler to run for this
    //  IRQ, and then finally, run it 
    handler = interrupt_handlers[regs.int_no - 32];
    if (handler)
    {
        handler(regs);
    }

    // If the IDT entry that was invoked was greater than 40
    //  (meaning IRQ8 - 15), then we need to send an EOI to
    //  the slave controller 
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    // In either case, we need to send an EOI to the master
    //  interrupt controller too 
    outb(0x20, 0x20);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         End of file isr.c                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
