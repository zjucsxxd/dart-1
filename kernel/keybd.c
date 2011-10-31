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

#include "keybd.h"      // keybd interface

unsigned shiftstate = 0;
unsigned char* kbdbuff;
unsigned char tempbuf;
/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        // Either Shift, Alt or Ctrl was pressed
        //puts((char *)(scancode));
        if (scancode == 0x2a)
        { 
            printf("Scancode->0x2a->Shift pressed!");
            shiftstate = 1;
        }
        else if (scancode == 0xaa)
        {
            shiftstate = 0;
        }
    }
    else
    {
        get_stream(kbdus[scancode]);
    }
}

// This method bellow is just for testing purposes
// Once the keyboard driver works perfectly, this won't
// print any characters to the screen, instead, this will
// be handled by the functions bellow "keyboard_install()
char* get_stream(char character)
{
    char backspace = '\b';
    kput(character); // Print the character to the screen
    kbdbuff = kbdbuff + character;
    if (character == '\b') kbdbuff = kbdbuff + backspace;
    if (character == '\n') kbdbuff = "";
    regs = (char*)(kbdbuff);
    return regs;
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    register_interrupt_handler(1, keyboard_handler);
}

//Gets a key
unsigned char getch()
{
     unsigned char getch_char;
     
     if(kbdus[inb(0x33)] != 0) //Not empty
        outb(0x33,0xf4); //Clear buffer
     
     while(kbdus[inb(0x33)] == 0); //While buffer is empty
        getch_char = kbdus[inb(0x33)];//0x60)];
     kput(getch_char);
     outb(0x33,0xf4); //Leave it emptying
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
     buffer[kb_count++] = 0; 
     for(;kb_count; kb_count--)
     {
          buffer2[kb_count] = buffer[kb_count];
     }
     return;
}
