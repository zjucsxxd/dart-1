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

#ifndef KEYBD_H
#define KEYBD_H

// Libraries to include to the driver's
// source code.
#include "common.h"     // inb, outb
#include "screen.h"     // kput
#include "stdio.h"      // printf
#include "keymap-us.h"  // keyboard keymap

char* regs;

// Keyboard driver functions
extern void keyboard_install();
extern char* get_stream(char character);

unsigned char getch();
char* gets();
static void do_gets();

// Keyboard driver variables
volatile char* buffer2;
volatile char* buffer;        //For storing strings
volatile u32int kb_count = 0; //Position in buffer
volatile int gets_flag = 0;
#endif
