// Author: Constantine Apostolou
// e-mail: conmarap@osarena.net

// Out port B function
#include <comfunc.h>
#include <def.h>

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

