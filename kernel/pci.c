/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - This is used to descover PCI      *
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

#include "screen.h"     // Contains the kprintf, kput and kclear 
#include "pci.h"        // Contains the interface for communicating with BIOS
#include "stdio.h"      // Contains the stdio functions 
#include "common.h"     // Contains the commonly used functions 

PCI_BIOS_t *pci_bios;

static u32int pci_read(int bus, int dev, int func, int reg);
static PCI_Device_t* pci_readdevice(int bus, int dev, int func);
static int pci_find_bios();
static PCI_Args_t* pci_fill_struct(PCI_Device_t* dev);

//This will scan the PCI Bus for all the devices
//and will determine which drivers need to be loaded
//or which are needed

//Reads DWORD config register
static u32int pci_read(int bus, int dev, int func, int reg)
{
	outb(0xCF8,
	    ((unsigned long) 0x80000000 | (bus << 16) | (dev << 11) |
	     (func << 8) | reg));
	return inb(0xCFC);
}


PCI_Device_t* ret;

static PCI_Device_t* pci_readdevice(int bus, int dev, int func)
{ 
	int place, total = sizeof(PCI_Device_t) / sizeof(unsigned long);

	for (place = 0; place < total; place++)
	{
		((u32int*)ret)[place] =  pci_read(bus, dev, func, (place * sizeof (unsigned long)));
	}
  
	return ret;
}

static int pci_find_bios()
{
	unsigned long addr;
	unsigned char crc;
	int i;

	for (addr = 0xE0000; addr < 0xFFFFF; addr += 0x10)
	{
		pci_bios = (PCI_BIOS_t *) addr;
		if (pci_bios->Magic == 0x5F32335F)
		{
			for (i = 0, crc = 0; i < (pci_bios->Pages * 16); i++)
			{
				crc += *((unsigned char *) (addr + i));
			}
			if (crc == 0)
			{ 
				//found bios
				printf("* PCI BIOS found at 0x%x\n",addr);
				return 0;
			}
		}
	}
	kprintf("* PCI BIOS not found\n");
	return 1;
}

void init_pci()
{    
     //Check for the presence of BIOS
     if(pci_find_bios())
     {
          kprintf("*   PCI not found\n");
          return;
     }
     
     kprintf("*   Enumerating hardware\n");
     
     ret = (PCI_Device_t*)kmalloc(sizeof(PCI_Device_t));
     
     //Enumerate devices and if a driver is available, load it.
     //For booting we will only load the required drivers
     
     #ifdef DEBUG
     int invalid;
     #endif

     int bus, device, function;
     static int count = 0;
     PCI_Device_t *tmp;
     
     for (bus = 0; bus <= 0xFF; bus++)
     {
         for (device = 0; device < 32; device++)
	     {
             for (function = 0; function < 8; function++)
             {
	             tmp = pci_readdevice(bus, device, function);
	             if((tmp->VendorID == 0) || (tmp->VendorID == 0xFFFF) || (tmp->DeviceID == 0xFFFF)) {}
	             else
				 { 
					 count++; 
                         
                     #ifdef DEBUG
                     invalid = 0;
                     #endif
                         
                     switch(tmp->ClassCode)
                     {
                         //Pre-PCI Items
                         case 0x00:
                             if(tmp->SubClass == 0x01) //VGA
                             {
								 kprintf("    VGA driver loading (legacy)... \n");
                                 break;
                             }
                             break;
                         case 0x01:
                             if(tmp->SubClass == 0x01) //IDE (And possibly CD?)
                             {
								 kprintf("    ATA (IDE) driver loading... [TODO]\n");
                                 break;
                             }
                             if(tmp->SubClass == 0x02) //Floppy
                             {
								 kprintf("    Floppy driver loading (legacy)... [TODO]\n");
                                 break;
							 }
                             if(tmp->SubClass == 0x80)
                             {
								 kprintf("    Mass storage device controller driver loading... [TODO]\n");
								 break;
							 }
							 break;
							 //NIC's
						 case 0x02:
							 if(tmp->SubClass == 0x00)
							 {
								 //We need to find out which chipset here
								 kprintf("    Ethernet driver loading\n");
								 switch(tmp->VendorID)
								 {
									 /* Realtek RTL8139 and clones */
								 case 0x10EC:
									 if(tmp->DeviceID == 0x8139 || tmp->DeviceID == 0x8138)
									 {
										 kprintf("    Realtek RTL8139 Discovered\n");
									 }
									 break;
								 case 0x1113:
									 if(tmp->DeviceID == 0x1211)
									 {
										 kprintf("    SMC1211TX EZCard 10/100 (RTL8139 clone) Discovered\n");
									 }
									 break;
								 case 0x1500:
									 if(tmp->DeviceID == 0x1360)
									 {
										 kprintf("    Delta Electronics 8139 10/100BaseTX (RTL8139 clone) Discovered\n");
									 }
									 break;
								 case 0x4033:
									 if(tmp->DeviceID == 0x1360)
									 {
										 kprintf("    Addtron Technology 8139 10/100BaseTX (RTL8139 clone) Discovered\n");
									 }
									 break;
								 case 0x1186:
									 if(tmp->DeviceID == 0x1300)
									 {
										 kprintf("    D-Link DFE-538TX (RTL8139 clone) Discovered\n");
									 }
									 break;
								 default:
									 kprintf("    Unknown ethernet card, unable to load driver\n");
									 break;
								 }
								 break;
							 }
							 if(tmp->SubClass == 0x80)
							 {
								 kprintf("    Network controller driver loading\n");
								 break;
							 }
							 break;
							 //Video
						 case 0x03:
							 if(tmp->SubClass == 0x00)
							 {
								 kprintf("    VGA driver loading");
								 break;
							 }
							 if(tmp->SubClass == 0x01)
							 {
								 kprintf("    XGA/VESA driver loading\n");
								 break;
							 }
							 break;
							 //Bridge Devices
						 case 0x06:          
							 if(tmp->SubClass == 0x00)
							 {
								 kprintf("    Host/PCI Bridge discovered - Driver activated\n");
								 break;
							 }
							 if(tmp->SubClass == 0x01)
							 {
								 kprintf("    PCI/ISA Bridge driver loading\n");
								 break;
							 }
							 if(tmp->SubClass == 0x04)
							 {
								 kprintf("    PCI/PCI Bridge driver loading\n");
								 break;
							 }
							 break;
							 //Other
						 case 0x0C:
							 if(tmp->SubClass == 0x03)
							 {
								 kprintf("    USB controller driver loading\n");
								 break;
							 }
							 break;
							 //None of the above
						 default:
#ifdef DEBUG
							 invalid++;
#endif
							 break;
                     }          
#ifdef DEBUG     
					 if(invalid == 0)
					 { 
						 printf("    VID:0x%x DID:0x%x\n",tmp->VendorID,tmp->DeviceID);
						 printf("    CC:0x%x SCC:0x%x\n",tmp->ClassCode,tmp->SubClass);
						 printf("    BAR0:0x%x BAR1:0x%x BAR2:0x%x\n",tmp->BAR0,tmp->BAR1,tmp->BAR2);
						 printf("    BAR3:0x%x BAR4:0x%x BAR5:0x%x\n",tmp->BAR3,tmp->BAR4,tmp->BAR5);
						 printf("    IRQ:%d\n",tmp->IRQ);
				  	 }
#endif
              	 }
         	 }
	     }
     }
     
     printf("*   Pegasus found %d devices\n",count);
	 kfree(ret);
}

static PCI_Args_t* pci_fill_struct(PCI_Device_t* dev)
{
	PCI_Args_t* ret;
     
	//Vendor and Device ID's
    ret->VendorID = dev->VendorID;
    ret->DeviceID = dev->DeviceID;
     
    //IRQ
    ret->IRQ = dev->IRQ;
     
    ret->BAR0 = dev->BAR0;
    ret->BAR1 = dev->BAR1;
    ret->BAR2 = dev->BAR2;
    ret->BAR3 = dev->BAR3;
    ret->BAR4 = dev->BAR4;
    ret->BAR5 = dev->BAR5;
     
    return ret;
}
