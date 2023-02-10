#include "pe_arch.h"

static void pe_parser(FILE * fp)
{
	unsigned short tok[1024];
	
	advance(tok, (0x40 - 4) + 10, fp);
	unsigned short toki = 0;
	advance(&toki, 2, fp);
	
	print_label("Arch");
	bpair_parser(toki, pe_arches, sizeof pe_arches / sizeof(bpr), "arch");
}
