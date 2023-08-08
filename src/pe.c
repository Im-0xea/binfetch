#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"
#include "pe_arch.h"

void pe_parser(FILE * fp)
{
	unsigned short tok[1024];

	advance(tok, 10, fp);
	unsigned short toki = 0;
	advance(&toki, 2, fp);

	bpair_parser(ibuffer[buffer_pos], toki, pe_arches, sizeof pe_arches / sizeof(struct bpr), "arch");

	add_label("Arch", ibuffer[buffer_pos++]);
}
