#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "info.h"
#include "pair.h"
#include "bin_op.h"

#include "pe_arch.h"

void pe_parser(FILE * fp, base * bs)
{
	unsigned short tok[1024];
	
	advance(tok, 10, fp);
	unsigned short toki = 0;
	advance(&toki, 2, fp);
	
	bpair_parser(bs->arch, toki, pe_arches, sizeof pe_arches / sizeof(bpr), "arch");
	
	add_label("Arch", bs->arch);
}
