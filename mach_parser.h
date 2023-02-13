#include "mach_arch.h"
#include "mach_type.h"

static void mach_parser(FILE * fp, int bit, int end)
{
	fbyte tok = 0;
	
	advance(&tok, 4, fp);
	
	print_label("Arch");
	
	bpair_parser(tok, mach_arches, sizeof mach_arches / sizeof(bpr), "cpu type");
	
	fbyte cls = (tok >> 24);
	print_label("Class");
	
	if (cls == 0x00)	
	{
		printf("32 bit\n");
	}
	else if (cls == 0x1)
	{
		printf("64 bit\n");
	}
	else if (cls == 0x2)
	{
		printf("LP 32\n");
	}
	else
	{
		printf("unknown (%x)", cls);
	}
	
	print_label("Endianness");
	if (end == 0)
	{
		printf("little endian\n");
	}
	else if (end == 1)
	{
		printf("big endian\n");
	}
	else
	{
		printf("unknown endian\n");
	}
	
	advance(&tok, 4, fp);
	advance(&tok, 4, fp);
	
	print_label("Type");
	
	spair_parser(tok, mach_types, sizeof mach_types / sizeof(spr), "type");
}
