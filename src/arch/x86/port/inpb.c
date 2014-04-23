// Author: Constantine Apostolou
// e-mail: conmarap@osarena.net

// In port B function
#include <comfunc.h>
#include <def.h>

uint8_t  inpb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
