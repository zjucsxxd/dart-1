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
#include "keymap-us.h"

extern unsigned long g_csr_x, g_csr_y;
extern unsigned long g_wd, g_ht;
extern int graphical_mode;

extern window_t current_window;
extern int shell_csr_x;
extern int shell_csr_y;

volatile int shift_flag=0;
volatile int caps_flag=0;

volatile char* buffer; //For storing strings
volatile char* buffer2;
volatile u32int kb_count = 0; //Position in buffer
volatile int gets_flag = 0;

unsigned short ltmp;
int ktmp = 0;

static void do_gets();

/* Handles the keyboard interrupt */
static void keyboard_handler(registers_t* regs)
{
    unsigned char scancode;

    //Read scancode
    scancode = inb(0x60);
    
    switch (scancode)
    {
           case 0x3A:
                /* CAPS_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 4;
                outb(0x60,ltmp);
                
                if(caps_flag)
                caps_flag=0;
                else
                caps_flag=1;
                break;
           case 0x45:
                /* NUM_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 2;
                outb(0x60,ltmp);
                break;
           case 0x46:
                /* SCROLL_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 1;
                outb(0x60,ltmp);
                break;
           case 60: /* F12 */
                reboot();
                break;
           default:
                break;
    }

    if (scancode & 0x80)
    {
        //Key release
        
        //Left and right shifts
        if (scancode - 0x80 == 42 || scancode - 0x80 == 54)
			shift_flag = 0;
    }
	else 
	{   
        //Keypress (normal)
        
        //Shift
        if (scancode == 42 || scancode == 54)
		{
			shift_flag = 1;
			return;
		}
        
        //Gets()
        if(kbdus[scancode] == '\n')
        {
             if(gets_flag == 0) do_gets();
             gets_flag++;
             for(;kb_count; kb_count--)
                  buffer[kb_count] = 0;              
        } 
		else 
		{
             if(kbdus[scancode] == '\b')
             {
                  if(kb_count)
                  buffer[kb_count--] = 0;
             } else {
                  buffer[kb_count++] = kbdus[scancode];
             }
                  
        } 
        
             
	    monitor_put(kbdus[scancode]);
        return;
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, &keyboard_handler);
}

//Gets a key
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
     while(gets_flag == 0);
     return (char*)buffer2;
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
