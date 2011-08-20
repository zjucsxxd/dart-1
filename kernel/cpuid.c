/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Heavily based on open source files*                    *
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
#include "cpuid.h"

// Application entry function
int detect_cpu(void) 
{ 
	unsigned long ebx, unused;
	cpuid(0, unused, ebx, unused, unused);
	switch(ebx) {
		case 0x756e6547: // Intel Code was detected
		do_intel();
		break;
		case 0x68747541: // AMD Code was detected
		do_amd();
		break;
		default:         // Unkown Code was detected
		kprintf("Unknown x86 CPU Detected\n");
		break;
	}
        kprintf("\n\n");
	return 0;
}

// Intel Specific brand list
char *Intel[] = {
	"Brand ID Not Supported.", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Intel(R) Pentium(R) III Xeon(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) III processor-M", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Xeon(R) Processor", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) 4 processor-M", 
	"Mobile Intel(R) Pentium(R) Celeron(R) processor", 
	"Reserved", 
	"Mobile Genuine Intel(R) processor", 
	"Intel(R) Celeron(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Celeron(R) processor", 
	"Mobile Geniune Intel(R) processor", 
	"Intel(R) Pentium(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor"
};

/* This table is for those brand strings that have two values depending on the processor signature. It should have the same number of entries as the above table. */
char *Intel_Other[] = {
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Celeron(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved"
};

/* Intel-specific information */
int do_intel(void) 
{
        kprintf("* CPU found: Intel Specific Features\n");
	
	unsigned long eax, ebx, ecx, edx, max_eax, signature, unused;
	int model, family, type, brand, stepping, reserved;
	int extended_family = -1;
	
	cpuid(1, eax, ebx, unused, unused);
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	type = (eax >> 12) & 0x3;
	brand = ebx & 0xff;
	stepping = eax & 0xf;
	reserved = eax >> 14;
	signature = eax;
	
	//kprintf("Type ");
	//kprintf_dec(type);
	//kprintf(" - ");
	kprintf("*   ");
	
	switch(type) 
	{
		case 0:
		kprintf("Original OEM");
		break;
		case 1:
		kprintf("Overdrive");
		break;
		case 2:
		kprintf("Dual-capable");
		break;
		case 3:
		kprintf("Reserved");
		break;
	}
	
	kprintf("\n");
	
	//kprintf("Family ");
	//kprintf_dec(family);
	//kprintf(" - ");
	
	kprintf("*   ");

	switch(family) {
		case 3:
		kprintf("i386");
		break;
		case 4:
		kprintf("i486");
		break;
		case 5:
		kprintf("Pentium");
		break;
		case 6:
		kprintf("Pentium Pro");
		break;
		case 15:
		kprintf("Pentium 4");
	}
	
	kprintf("\n");
	
	if(family == 15) 
	{
		extended_family = (eax >> 20) & 0xff;
		kprintf("Extended family ");
		kprintf_dec(extended_family);
	    kprintf("\n");
	}
	//kprintf("Model ");
	//kprintf_dec(model);
	//kprintf(" - ");
	kprintf("*   ");
	
	switch(family) 
	{
		case 3:
		break;
		case 4:
		switch(model) 
		{
			case 0:
			case 1:
			kprintf("DX");
			break;
			case 2:
			kprintf("SX");
			break;
			case 3:
			kprintf("487/DX2");
			break;
			case 4:
			kprintf("SL");
			break;
			case 5:
			kprintf("SX2");
			break;
			case 7:
			kprintf("Write-back enhanced DX2");
			break;
			case 8:
			kprintf("DX4");
			break;
		}
		break;
		case 5:
		switch(model) 
		{
			case 1:
			kprintf("60/66");
			break;
			case 2:
			kprintf("75-200");
			break;
			case 3:
			kprintf("for 486 system");
			break;
			case 4:
			kprintf("MMX");
			break;
		}
		break;
		case 6:
		switch(model) 
		{
			case 1:
			kprintf("Pentium Pro");
			break;
			case 3:
			kprintf("Pentium II Model 3");
			break;
			case 5:
			kprintf("Pentium II Model 5/Xeon/Celeron");
			break;
			case 6:
			kprintf("Celeron");
			break;
			case 7:
			kprintf("Pentium III/Pentium III Xeon - external L2 cache");
			break;
			case 8:
			kprintf("Pentium III/Pentium III Xeon - internal L2 cache");
			break;
		}
		break;
		case 15:
		break;
	}
	kprintf("\n");
	cpuid(0x80000000, max_eax, unused, unused, unused);
	/* Quok said: If the max extended eax value is high enough to support the processor brand string
	(values 0x80000002 to 0x80000004), then we'll use that information to return the brand information. 
	Otherwise, we'll refer back to the brand tables above for backwards compatibility with older processors. 
	According to the Sept. 2006 Intel Arch Software Developer's Guide, if extended eax values are supported, 
	then all 3 values for the processor brand string are supported, but we'll test just to make sure and be safe. */
	if(max_eax >= 0x80000004) 
	{
                kprintf("* ");
		kprintf("Brand: ");
		if(max_eax >= 0x80000002) 
		{
			cpuid(0x80000002, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000003) 
		{
			cpuid(0x80000003, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000004) 
		{
			cpuid(0x80000004, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		
		kprintf("\n");
	} 
	else if(brand > 0) 
	{
	
		kprintf("Brand ");
		kprintf_dec(brand);
		kprintf(" - ");
		
		if(brand < 0x18) {
			if(signature == 0x000006B1 || signature == 0x00000F13) 
                        {
				kprintf(Intel_Other[brand]);
                                kprintf("\n");
			} 
			else 
			{
				kprintf(Intel[brand]);
                                kprintf("\n");
			}
		} 
		else 
		{
			kprintf("Reserved\n");
		}
	}
	
	kprintf("*   ");
	kprintf("Stepping: ");
	kprintf_dec(stepping);
	kprintf("\n*   ");
	kprintf("Reserved: ");
	kprintf_dec(reserved);
	
	return 0;
}

/* Print Registers */
void printregs(int eax, int ebx, int ecx, int edx) 
{
	int j;
	char string[17];
	
	string[16] = '\0';
	
	for(j = 0; j < 4; j++) {
		string[j] = eax >> (8 * j);
		string[j + 4] = ebx >> (8 * j);
		string[j + 8] = ecx >> (8 * j);
		string[j + 12] = edx >> (8 * j);
	}
	kprintf(string);
}

/* AMD-specific information */
int do_amd(void) 
{
        kprintf("* CPU found: AMD Specific Features\n");
	
	unsigned long extended, eax, ebx, ecx, edx, unused;
	int family, model, stepping, reserved;
	cpuid(1, eax, unused, unused, unused);
	
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	stepping = eax & 0xf;
	reserved = eax >> 12;
	
	kprintf("Family ");
	kprintf_dec(family);
	kprintf(" - ");
	
	kprintf("Model: ");
	kprintf_dec(model);
	kprintf(" - ");
	
	switch(family) {
		case 4:
	        kprintf("*   ");
		kprintf("486 Model ");
		kprintf_dec(model);
		break;
		case 5:
		switch(model) 
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 6:
			case 7:
	                kprintf("*   ");
			kprintf("K6 Model ");
			kprintf_dec(model);
			break;
			case 8:
	                kprintf("*   ");
			kprintf("K6-2 Model 8");
			break;
			case 9:
	                kprintf("*   ");
			kprintf("K6-III Model 9");
			break;
			default:
	                kprintf("*   ");
			kprintf("K5/K6 Model ");
			kprintf_dec(model);
			break;
		}
		break;
		case 6:
		switch(model) 
		{
			case 1:
			case 2:
			case 4:
	                kprintf("*   ");
			kprintf("Athlon Model ");
			kprintf_dec(model);
			break;
			case 3:
	                kprintf("*   ");
			kprintf("Duron Model 3");
			break;
			case 6:
	                kprintf("*   ");
			kprintf("Athlon MP/Mobile Athlon Model 6");
			break;
			case 7:
	                kprintf("*   ");
			kprintf("Mobile Duron Model 7");
			break;
			default:
	                kprintf("*   ");
			kprintf("Duron/Athlon Model ");
			kprintf_dec(model);
			break;
		}
		break;
	}
	
	kprintf("]\n");
	cpuid(0x80000000, extended, unused, unused, unused);
	
	if(extended == 0) 
	{
		return 0;
	}
	if(extended >= 0x80000002) 
	{
		unsigned int j;
	        kprintf("*   ");
		kprintf("Detected Processor Name: ");
		
		for(j = 0x80000002; j <= 0x80000004; j++) 
		{
			cpuid(j, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		kprintf("\n");
	}
	if(extended >= 0x80000007) 
	{
		cpuid(0x80000007, unused, unused, unused, edx);
		
		if(edx & 1) 
		{
			kprintf("Temperature Sensing Diode Detected!\n");
		}
	}
	
	kprintf("*   ");
	kprintf("Stepping: ");
	kprintf_dec(stepping);
	kprintf("\n*   ");
	kprintf("Reserved: ");
	kprintf_dec(reserved);
	
	return 0;
}
