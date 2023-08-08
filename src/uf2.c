#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"

#include "binfetch.h"
#include "bin_op.h"
#include "pair.h"

void uf2_parser(FILE * fp)
{
	uint8_t tok[28];
	advance(tok, 28, fp);
	
	sprintf(ibuffer[buffer_pos], "0x%02x", tok[1]);
	add_label("Flashing Addr", ibuffer[buffer_pos++]);
	
	sprintf(ibuffer[buffer_pos], "0x%02x", tok[2]);
	add_label("Data size", ibuffer[buffer_pos++]);
	
	sprintf(ibuffer[buffer_pos], "0x%02x", tok[4]);
	add_label("Block Num", ibuffer[buffer_pos++]);
	
	if (tok[0] & 0x00002000)
	{
		sprintf(ibuffer[buffer_pos], "0x%02x", tok[5]);
		add_label("Family ID", ibuffer[buffer_pos++]);
	}
}
