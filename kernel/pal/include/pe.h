/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Interface for handling *.PE files *
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

#ifndef PE_H
#define PE_H

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    unsigned short e_magic;         // Magic number (Should be MZ)
    unsigned short e_cblp;          // Bytes on last page of file
    unsigned short e_cp;            // Pages in file
    unsigned short e_crlc;          // Relocations
    unsigned short e_cparhdr;       // Size of header in paragraphs
    unsigned short e_minalloc;      // Minimum extra paragraphs needed
    unsigned short e_maxalloc;      // Maximum extra paragraphs needed
    unsigned short e_ss;            // Initial (relative) SS value
    unsigned short e_sp;            // Initial SP value
    unsigned short e_csum;          // Checksum
    unsigned short e_ip;            // Initial IP value
    unsigned short e_cs;            // Initial (relative) CS value
    unsigned short e_lfarlc;        // File address of relocation table
    unsigned short e_ovno;          // Overlay number
    unsigned short e_res[4];        // Reserved words
    unsigned short e_oemid;         // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;       // OEM information; e_oemid specific
    unsigned short e_res2[10];      // Reserved words
    long   e_lfanew;        // File address of new exe header
} PEHeader_t, *PPEHeader_t;

typedef struct _IMAGE_FILE_HEADER {
    unsigned short  Machine;
    unsigned short  NumberOfSections;
    unsigned long   TimeDateStamp;
    unsigned long   PointerToSymbolTable;
    unsigned long   NumberOfSymbols;
    unsigned short  SizeOfOptionalHeader;
    unsigned short  Characteristics;
} PEFileHeader_t, *PPEFileHeader_t;

typedef struct _IMAGE_OPTIONAL_HEADER {
    unsigned short  Magic;
    unsigned char   MajorLinkerVersion;
    unsigned char   MinorLinkerVersion;
    unsigned long   SizeOfCode;
    unsigned long   SizeOfInitializedData;
    unsigned long   SizeOfUninitializedData;
    unsigned long   AddressOfEntryPoint;			//This is where we call our program
    unsigned long   BaseOfCode;
    unsigned long   BaseOfData;
    //A few more NT fields here
} PEOptionalHeader_t, *PPEOptionalHeader_t;

#endif
