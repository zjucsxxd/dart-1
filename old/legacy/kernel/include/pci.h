/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the interface for PCI     *
 *                                   - drivers and devices               *
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

#ifndef PCI_H
#define PCI_H

typedef struct
{
	unsigned long Magic;
	unsigned long EntryPoint;
	unsigned char Version;
	unsigned char Pages;
	unsigned char CRC;
} PCI_BIOS_t;

typedef struct
{
	unsigned short VendorID;  
	unsigned short DeviceID;
	unsigned short CommandReg;
	unsigned short StatusReg;
	unsigned char RevisionID;
	unsigned char ProgIF;
	unsigned char SubClass;
	unsigned char ClassCode;
	unsigned char CachelineSize;
	unsigned char Latency;
	unsigned char HeaderType;
	unsigned char BIST;

	unsigned int BAR0; //Base Address Register
	unsigned int BAR1;
	unsigned int BAR2;
	unsigned int BAR3;
	unsigned int BAR4;
	unsigned int BAR5;
	unsigned int CardbusCISPtr;
	unsigned short SubVendorID; //Subsystem
	unsigned short SubDeviceID;
	unsigned int ExRomAddress; //Expansion ROM
	unsigned int Reserved1;
	unsigned int Reserved2;
	unsigned char IRQ; //IRQ number
	unsigned char PIN; //IRQ PIN number
	unsigned char MinGrant;
	unsigned char MaxLatency;
} PCI_Device_t;

typedef struct
{
	unsigned short VendorID;  
	unsigned short DeviceID;
	unsigned int BAR0;
	unsigned int BAR1;
	unsigned int BAR2;
	unsigned int BAR3;
	unsigned int BAR4;
	unsigned int BAR5;
	unsigned char IRQ;
} PCI_Args_t;

// I/O Ports (32 bit)
#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

void init_pci();

#endif
