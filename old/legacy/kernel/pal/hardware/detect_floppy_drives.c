/*********************************************************************
Codename Pegasus OS - Floppy drive detection
*********************************************************************/
#include "common.h"
#include "stdio.h"

void detect_floppy_drives()
{
	unsigned char c;
	unsigned char a, b;
	char *drive_type[5] = {
		"no floppy drive",
		"360kb 5.25in floppy drive",
		"1.2mb 5.25in floppy drive",
		"720kb 3.5in",
		"1.44mb 3.5in",
		"2.88mb 3.5in" };

	outb(0x70, 0x10);
	c = inb(0x71);

	a = c >> 4; // get the high nibble
	b = c & 0xF; // get the low nibble by ANDing out the high nibble

	printf("*   Floppy drive A->");
	printf(drive_type[a]);
	//printf("\nFloppy drive B is an:\n");
	//printf(drive_type[b]);
	printf("\n");
};
