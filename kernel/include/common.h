/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines typedefs and some global  *
                                     - functions.                        *
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
#ifndef COMMON_H
#define COMMON_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Typedefs to standarize sizes across the 32-bit x86  *
 *                   computer platform                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;
typedef unsigned long  u64int;
typedef          long  s64int;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *    Define panic probability and handle it           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *              Define other functions                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

void reboot();

#endif 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       End of file common.h                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
