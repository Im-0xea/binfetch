#include <elf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gelf.h>
#include <libelf.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"
#include "elf_arch.h"
#include "elf_type.h"
#include "elf_osabi.h"

void elf_parser(FILE * fp)
{
	Elf *e;
	GElf_Ehdr ehdr;

	if (elf_version(EV_CURRENT) == EV_NONE) {
		printf("failed to initilize libelf: %s", elf_errmsg(-1));
		return;
	}

	if ((e = elf_begin(fileno(fp), ELF_C_READ, NULL)) == NULL) {
		printf("elf_begin() failed: %s", elf_errmsg(-1));
		return;
	}

	if (elf_kind(e) != ELF_K_ELF) {
		return;
	}

	if (gelf_getehdr(e, &ehdr) == NULL) {
		printf("getehdr failed");
		return;
	}

	int c = gelf_getclass(e);
	if (c == ELFCLASSNONE) {
		strcpy(ibuffer[buffer_pos], "unknown");
		add_label("Class", ibuffer[buffer_pos++]);
		return;
	} else if (c == ELFCLASS32) {
		strcpy(ibuffer[buffer_pos], "32 bit");
	} else {
		strcpy(ibuffer[buffer_pos], "64 bit");
	}
	add_label("Class", ibuffer[buffer_pos++]);


	if (ehdr.e_ident[5] == 0x01) {
		strcpy(ibuffer[buffer_pos], "little endian");
	} else if (ehdr.e_ident[5] == 0x02) {
		strcpy(ibuffer[buffer_pos], "big endian");
	} else {
		strcpy(ibuffer[buffer_pos], "unknown endian");
		add_label("Endian", ibuffer[buffer_pos++]);
		return;
	}
	add_label("Endian", ibuffer[buffer_pos++]);

	if (ehdr.e_ident[6] == 0x01) {
		strcpy(ibuffer[buffer_pos], "ELFv1");
	} else if (ehdr.e_ident[6] == 0x00) {
		strcpy(ibuffer[buffer_pos], "noncompliant");
	}
	add_label("Version", ibuffer[buffer_pos++]);

	spair_parser(ibuffer[buffer_pos], ehdr.e_ident[7], osabis, sizeof osabis / sizeof(struct spr), "abi");

	add_label("OS ABI", ibuffer[buffer_pos++]);

	if (ehdr.e_ident[8] != 0) {
		sprintf(ibuffer[buffer_pos], "%d", ehdr.e_ident[8]);
		add_label("ABI Version", ibuffer[buffer_pos]);
	} else {
		ibuffer[buffer_pos][0] = '\0';
	}
	
	spair_parser(ibuffer[buffer_pos], ehdr.e_type, elf_types, sizeof elf_types / sizeof(struct spr), "type");

	add_label("Type", ibuffer[buffer_pos++]);


	spair_parser(ibuffer[buffer_pos], ehdr.e_machine, elf_arches, sizeof elf_arches / sizeof(struct spr), "arch");

	add_label("Arch", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%jx", ehdr.e_entry);
	add_label("Entry", ibuffer[buffer_pos++]);


	sprintf(ibuffer[buffer_pos], "0x%02x", ehdr.e_ehsize);

	add_label("ELF Header Size", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%jx (size 0x%02x entries %d)", ehdr.e_phoff, ehdr.e_phentsize, ehdr.e_phnum);
	add_label("Pgm Table", ibuffer[buffer_pos++]);

	sprintf(ibuffer[buffer_pos], "0x%jx (size 0x%02x entries %d names %d)", ehdr.e_shoff, ehdr.e_shentsize, ehdr.e_shnum, ehdr.e_shstrndx);
	add_label("Sec Table", ibuffer[buffer_pos++]);

	rewind(fp);
	fseek(fp, ehdr.e_shoff, SEEK_SET);
	int current_sec = 0;
	bool unstriped = 0;
	unsigned int sec[15];
	for (; current_sec < ehdr.e_shnum; ++current_sec) {
		advance(sec, ehdr.e_shentsize, fp);
		if (sec[1] == 0x2) {
			unstriped = true;
		}
	}

	if (unstriped) {
		strcpy(ibuffer[buffer_pos], "unstripped");
	} else {
		strcpy(ibuffer[buffer_pos], "stripped");
	}

	add_label("Striping", ibuffer[buffer_pos++]);
	elf_end(e);
}
