 /*
  * void count_memory (void)
  *
  * probes memory above 1mb
  *
  * last mod : 05sep98 - stuart george
  *            08dec98 - ""     ""
  *            21feb99 - removed dummy calls
  *
  */
  
  /// TODO: Make it CoPOS compatible
  
void count_memory(void)
{
	register ULONG *mem;
	ULONG	mem_count, a;
	USHORT	memkb;
	UCHAR	irq1, irq2;
	ULONG	cr0;
 
	/* save IRQ's */
	irq1=inb(0x21);
	irq2=inb(0xA1);
 
	/* kill all irq's */
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
 
	mem_count=0;
	memkb=0;
 
	// store a copy of CR0
	__asm__ __volatile("movl %%cr0, %%eax":"=a"(cr0))::"eax");
 
	// invalidate the cache
	// write-back and invalidate the cache
	__asm__ __volatile__ ("wbinvd");
 
	// plug cr0 with just PE/CD/NW
	// cache disable(486+), no-writeback(486+), 32bit mode(386+)
	__asm__ __volatile__("movl %%eax, %%cr0", ::
		"a" (cr0 | 0x00000001 | 0x40000000 | 0x20000000) : "eax");
 
	do {
		memkb++;
		mem_count += 1024*1024;
		mem= (ULONG*) mem_count;
 
		a= *mem;
		*mem= 0x55AA55AA;
 
          // the empty asm calls tell gcc not to rely on what's in its registers
          // as saved variables (this avoids GCC optimisations)
		asm("":::"memory");
		if (*mem!=0x55AA55AA) mem_count=0;
		else {
			*mem=0xAA55AA55;
			asm("":::"memory");
			if(*mem!=0xAA55AA55)
			mem_count=0;
		}
 
		asm("":::"memory");
		*mem=a;
 
	} while (memkb<4096 && mem_count!=0);
 
	__asm__ __volatile__("movl %%eax, %%cr0", :: "a" (cr0) : "eax");
 
	mem_end = memkb<<20;
	mem = (ULONG*) 0x413;
	bse_end= (*mem & 0xFFFF) <<6;
 
	outb(0x21, irq1);
	outb(0xA1, irq2);
}