// Author: Constantine Apostolou
// e-mail: conmarap@gmail.com

#include "include/def.h"
#include "include/comfunc.h"
#include "include/kstdio.h"
#include "include/multiboot.h"
#include "include/stdio.h"

// Kernel entry point
void _main (struct multiboot *mboot_ptr, uint32_t initial_stack) {
	kclear ();
	
	kprintf ("Boot successfull\n");
	int rand = 36;
	printf ("Number \"rand\" = %i.\n", rand);
	printf ("Initial kernel stack: %i\n", initial_stack);
	while (rand == 36) { }
}

