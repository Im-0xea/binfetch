#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"
#include "mach_arch.h"
#include "mach_type.h"

void mach_parser(FILE * fp, const uint32_t mag)
{
	//const int bit = mag >> 24 != 0xfe
	const int end = !mag ? mag << 24 == 0xce ? 0 : 1 : mag >> 24 == 0xce ? 0 : 1;

	uint32_t tok = 0;

	advance(&tok, 4, fp);

	bpair_parser(ibuffer[buffer_pos], tok, mach_arches, sizeof mach_arches / sizeof(struct bpr), "cpu type");

	add_label("Arch", ibuffer[buffer_pos++]);

	uint32_t cls = (tok >> 24);

	if (cls == 0x00) {
		strcpy(ibuffer[buffer_pos], "32 bit");
	} else if (cls == 0x1) {
		strcpy(ibuffer[buffer_pos], "64 bit"); 
	} else if (cls == 0x2) {
		strcpy(ibuffer[buffer_pos], "LP 32 bit");
	} else {
		sprintf(ibuffer[buffer_pos], "unknown (%x)", cls);
	}

	add_label("Class", ibuffer[buffer_pos++]);

	if (end == 0) {
		strcpy(ibuffer[buffer_pos], "little endian");
	} else if (end == 1) {
		strcpy(ibuffer[buffer_pos], "big endian");
	} else {
		strcpy(ibuffer[buffer_pos], "unknown endian");
	}

	add_label("Endian", ibuffer[buffer_pos++]);

	advance(&tok, 4, fp);
	advance(&tok, 4, fp);

	spair_parser(ibuffer[buffer_pos], tok, mach_types, sizeof mach_types / sizeof(struct spr), "type");

	add_label("Type", ibuffer[buffer_pos++]);
}

void mach_universal_parser(FILE * fp)
{
	uint32_t tok[4];
	int bin_count;
	fseek(fp, -4, SEEK_CUR);
	advance(tok, 4, fp);

	bin_count = ((int)tok[3]);
	sprintf(ibuffer[buffer_pos], "%d", tok[3]);
	add_label("Binary count", ibuffer[buffer_pos++]);

	uint32_t toki = 0;
	char *arches = malloc(bin_count*12);
	*arches = '\0';
	advance(&toki, 4, fp);
	for (int i=0; i<bin_count; i++) {
		while (toki != 0xfeedfacf && toki != 0xcefaedfe && toki != 0xcffaedfe)
			advance(&toki, 4, fp);
		advance(&toki, 4, fp);
		char buf[12];
		bpair_parser(buf, toki, mach_arches, sizeof mach_arches / sizeof(struct bpr), "cpu type");
		strcat(arches, buf);
		if (i != bin_count-1)
			strcat(arches, ", ");
	}
	strcpy(ibuffer[buffer_pos], arches);
	add_label("Arches", ibuffer[buffer_pos++]);
	free(arches);
}
