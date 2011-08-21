/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines some global functions.    *
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

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *    Write a byte out to the specified port.    *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *     Read a byte out to the specified port.    *
 * * * * * * * * * * * * * * * * * * * * * * * * */
u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *     Return the length of a string (char*)     *
 * * * * * * * * * * * * * * * * * * * * * * * * */
int strlen(char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *    Compare two strings. Should return -1 if   *
 *str1 < str2, 0 if they are equal or 1 otherwise*
 * * * * * * * * * * * * * * * * * * * * * * * * */
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // Exits the loop if it fails
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *        Copy len bytes from src to dest.       *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *      Write leng copies of val into dest.      *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 * Copy the NULL-terminated string src into dest *
 *                and return dest.               *
 * * * * * * * * * * * * * * * * * * * * * * * * */
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *Concatenate the NULL-terminated string src onto*
 *       the end of dest, and return dest.       *
 * * * * * * * * * * * * * * * * * * * * * * * * */
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *             Kernel Panic function             *
 * Disable interrupts, panic and halt the system *
 * * * * * * * * * * * * * * * * * * * * * * * * */
extern void panic(const char *message, const char *file, u32int line)
{
    asm volatile("cli"); // Disable interrupts.

    kprintf("PANIC! [");
    kprintf(message);
    kprintf("] -> ");
    kprintf(file);
    kprintf(":");
    kprintf_dec(line);
    kprintf("\n");
    for(;;); // Halt the system
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Kernel Panic - Assertion Failed function    *
 * Disable interrupts, panic and halt the system *
 * * * * * * * * * * * * * * * * * * * * * * * * */
extern void panic_assert(const char *file, u32int line, const char *desc)
{
    asm volatile("cli"); // Disable interrupts.

    kprintf("PANIC! ASSERTION-FAILED [");
    kprintf(desc);
    kprintf("] -> ");
    kprintf(file);
    kprintf(":");
    kprintf_dec(line);
    kprintf("\n");
    for(;;); // Halt the system
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       End of file common.c                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
