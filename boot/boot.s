; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
; Codename Pegasus Operating System - Main entry to the kernel. Also    *
;                                   - defines multiboot header          *
; Copyright (C) 2011  Constantine M. Apostolou                          *
; Author: Constantine M. Apostolou - conmarap@osarena.net               *
;                                                                       *
; This program is free software: you can redistribute it and/or modify  *
; it under the terms of the GNU General Public License as published by  *
; the Free Software Foundation, either version 3 of the License, or     *
; (at your option) any later version.                                   *
;                                                                       *
; This program is distributed in the hope that it will be useful,       *
; but WITHOUT ANY WARRANTY; without even the implied warranty of        *
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
; GNU General Public License for more details.                          *
;                                                                       *
; You should have received a copy of the GNU General Public License     *
; along with this program.  If not, see <http://www.gnu.org/licenses/>. *
; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

MBOOT_PAGE_ALIGN    equ 1<<0       ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1       ; Send the memory info to the kernel
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot value provided here

; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

mboot:
    dd  MBOOT_HEADER_MAGIC      ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS      ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM          ; To ensure that the above values are correct
    
    dd  mboot                   ; Location of this descriptor
    dd  code                    ; Start of kernel '.text' (code) section.
    dd  bss                     ; End of kernel '.data' section. After the last code in 
	                            ; mboot, the kernel debugger entry point will appear here.
    dd  end                     ; End of kernel.
    dd  start                   ; Kernel entry point (initial EIP).

[GLOBAL start]                  ; Kernel entry point. Which is found bellow
[EXTERN main]                   ; This is the entry point of our kernel's C code

start:
    ; Load multiboot information:
    push esp
    push ebx

    ; Execute the kernel:
    cli                         ; Disable interrupts. Note: This is done in the reboot function.
    call main                   ; call the kernel's main() function.
    jmp $                       ; Enter an infinite loop, to stop the processor
                                ; executing whatever crap is in the memory
                                ; after the execution of the kernel. This should be avoided though...
