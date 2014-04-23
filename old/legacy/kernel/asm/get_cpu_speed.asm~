; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
; Codename Pegasus Operating System - Contains the process routine      *
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

[GLOBAL __get_speed__]
__get_speed__:
   ;first do a cpuid command, with eax=1
   mov  eax,1
   cpuid
   test edx,byte 0x10      ; test bit #4. Do we have TSC ?
   jnz  detect_end         ; no ?, go to detect_end
   ;wait until the timer interrupt has been called.
   mov  ebx, ~[irq0_count]

[GLOBAL __wait_irq0__]
__wait_irq0__:

   cmp  ebx, ~[irq0_count]
   jz   wait_irq0
   rdtsc                   ; read time stamp counter
   mov  ~[tscLoDword], eax
   mov  ~[tscHiDword], edx
   add  ebx, 2             ; Set time delay value ticks.
   ; remember: so far ebx = ~[irq0]-1, so the next tick is
   ; two steps ahead of the current ebx ;)

[GLOBAL __wait_for_elapsed_ticks__]
__wait_for_elapsed_ticks__:

   cmp  ebx, ~[irq0_count] ; Have we hit the delay?
   jnz  wait_for_elapsed_ticks
   rdtsc
   sub eax, ~[tscLoDword]  ; Calculate TSC
   sbb edx, ~[tscHiDword]
   ; f(total_ticks_per_Second) =  (1 / total_ticks_per_Second) * 1,000,000
   ; This adjusts for MHz.
   ; so for this: f(100) = (1/100) * 1,000,000 = 10000
   mov ebx, 10000
   div ebx
   ; ax contains measured speed in MHz
   mov ~[mhz], ax