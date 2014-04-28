// Author: Constantine Apostolou
// e-mail: conmarap@gmail.com

#include <comfunc.h>
#include <def.h>
#include <kstdio.h>
#include <multiboot.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arch/cpuid.h>

// Kernel entry point
void _main(struct multiboot *mboot_ptr, uint32_t initial_stack) {
	kclear();

	kprintf("Boot successfull\n");
	int num = 36;
	printf("Integer \"num\" = %i.\n", num);
	printf("Initial kernel stack is: %i\n", initial_stack);

	detect_cpu();

	for(;;) { }
}
