/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Keyboard Driver                   *
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
#include "common.h"
#include "keybd.h"

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

extern int shell_csr_x;
extern int shell_csr_y;

volatile int shift_flag=0;
volatile int caps_flag=0;

volatile u32int kb_count = 0; //Position in buffer
volatile int gets_flag = 0;

unsigned short ltmp;
int ktmp = 0;

static void do_gets();

void keyboard_handler(registers_t* regs)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    switch(scancode)
    {
        case 0x3A:
            // Turn on the CAPS LED
            outb(0x60, 0xED);
            ltmp |= 4;
            outb(0x60, ltmp);
            kprintf("\nCAPS PRESSED");
            break;
    }

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        // This is used to see if the Ctrl Alt Shift keys were pressed
        if (scancode - 0x80 == 42 || scancode - 0x80 == 54) shift_flag = 0;
        /*
        else 
        {
            if(kbdus[scancode] == '\n')
            {
                if(gets_flag == 0) do_gets();
                gets_flag++;
                kput(kbdus[scancode]);
                for(;kb_count; kb_count--) buffer[kb_count]= 0;              
            }
            else 
            {
                if(kbdus[scancode] == '\b')
                {
                    if(kb_count)
                    buffer2[kb_count--] = 0;
                    kput(kbdus[scancode]);
                } 
                else 
                {
                    //Why not writeChar?
                    buffer2[kb_count++] = kbdus[scancode];
                    kput(kbdus[scancode]);
                }    
            }
        }*/
    }
    else
    {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */

        //kput(kbdus[scancode]);

        //Keypress (normal)
        
        //Shift
        if (scancode == 42 || scancode == 54)
	{
	      shift_flag = 1;
	      return;
	}
        
        if(kbdus[scancode] == '\n')
        {
             if(gets_flag == 0) do_gets();
             gets_flag++;
             for(;kb_count; kb_count--)
             {
                  buffer[kb_count] = 0;
                  //kput('0');
             }              
        } 
        else {
             if(kbdus[scancode] == '\b')
             {
                  if(kb_count)
                  buffer[kb_count--] = 0;
                  //kput('0');
             } else {
                  buffer[kb_count++] = kbdus[scancode];
             }
        } 
    }
    kput(kbdus[scancode]);
    outb(0x20,0x20);
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *This simple keyboard handler seems to function *
 *    TODO: Fix the keyboard handler - driver    *
 * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned shift_state = 0;
unsigned escaped = 0;

void KeyboardIsr()
{
    unsigned new_scan_code = inb(0x60);
    if(escaped)
    {
        new_scan_code += 256;
        escaped = 0;
    }
    switch(new_scan_code)
    {
        case 0x2A:
            shift_state = 1;
            kprintf("shift_state <- 1\n");
            break;
        case 0x3A:
            outb(0x60, 0xED);
            ltmp |= 4;
            outb(0x60, ltmp);
            kprintf("\nCAPS PRESSED");
            break;
        default:
            if(new_scan_code & 0x80)
                kprintf("Ignoring scancode\n");
            else kput(kbdus[new_scan_code]);
    }
}

void init_keyboard()
{
    kprintf("\nInitializing...");
    register_interrupt_handler(IRQ1, &keyboard_handler/*KeyboardIsr*/);
    kprintf("\nPS/2 keyboard initialisation");
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *        Gets a key using interrupts            *
 * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned char getch()
{
     unsigned char getch_char;
     
     if(kbdus[inb(0x60)] != 0) //Not empty
     outb(0x60,0xf4); //Clear buffer
     
     while(kbdus[inb(0x60)] == 0); //While buffer is empty
     getch_char = kbdus[inb(0x60)];
     outb(0x60,0xf4); //Leave it emptying
     return getch_char;
}

char* gets()
{ 
     gets_flag = 0;
     int i;
     i = 0;
     while(gets_flag == 0);
     //{
     //    buffer[i] = getch();
     //    if(buffer[i] == '\n') break;
     //    if(buffer[i] == 'd') break;
     //    else kput(buffer[i]);
     //    i++;
     //}
     //__asm__ __volatile__ ("hlt");
     return (char*)buffer;
}

static void do_gets()
{
     buffer[kb_count++] = 0; //Null terminated biatch!
     for(;kb_count; kb_count--)
     {
          buffer2[kb_count] = buffer[kb_count];
     }
     return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                        End of file keybd.c                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
