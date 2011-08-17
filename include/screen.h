/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - screen.h                          *
 * - Defines the interface for screen.h                                  *
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
#ifndef SCREEN_H
#define SCREEN_H

#include "common.h"

// Write a single character out to the screen.
void kput(char c);

// Clear the screen to all black.
void kclear();

// Output a null-terminated ASCII string to the monitor.
void kprintf(char *c);

// Output a hex value to the monitor.
void kprintf_hex(u32int n);

// Output a decimal value to the monitor.
void kprintf_dec(u32int n);

// kprintf module for text mode
void sprintf (const char *format, ...);

#endif // SCREEN_H
