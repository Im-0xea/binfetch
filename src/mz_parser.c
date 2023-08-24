#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"

void mz_parser(FILE * fp)
{
	uint8_t tok[16];

	advance(tok, 8, fp);

	sprintf(ibuffer[buffer_pos], "%d", tok[2]);
	add_label("Pages", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "%d", tok[3]);
	add_label("Relocs", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%02x", tok[4]);
	add_label("Header size", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%02x", tok[5]);
	add_label("Min Alloc", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%02x", tok[6]);
	add_label("Max Alloc", ibuffer[buffer_pos++]);
}
