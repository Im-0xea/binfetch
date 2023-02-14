#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "info.h"
#include "pair.h"
#include "bin_op.h"

#include "elf_arch.h"
#include "elf_type.h"
#include "elf_osabi.h"

const size_t max_tok = 1018;

void elf_parser(FILE * fp, base * bs, elf * as)
{
	
	byte tok[max_tok];
	
	advance(tok, 5, fp);
	
	int bits = 0 ;// 32 or 64
	
	if (tok[0] == 0x01)	
	{
		bits = 1;
		strcpy(bs->class, "32 bit");
	}
	else if (tok[0] == 0x02)
	{
		bits = 2;
		strcpy(bs->class, "64 bit");
	}
	else
	{
		strcpy(bs->class, "unknown");
	}
	
	add_label("Class", bs->class);
	
	if (tok[1] == 0x01)
	{
		strcpy(bs->endian, "little endian");
	}
	else if (tok[1] == 0x02)
	{
		strcpy(bs->endian, "big endian");
	}
	else
	{
		strcpy(bs->endian, "unknown endian");
	}
	
	add_label("Endian", bs->endian);
	
	
	if (tok[2] == 0x01)
	{
		strcpy(as->version, "ELFv1");
	}
	else if (tok[2] == 0x00)
	{
		strcpy(as->version, "noncompliant");
	}
	
	add_label("Version", as->version);
	
	spair_parser(as->osabi, tok[3], osabis, sizeof osabis / sizeof(spr), "abi");
	
	add_label("OS ABI", as->osabi);
	
	if (tok[4] != 0)
	{
		sprintf(as->osabi_v, "%d", tok[4]);
		add_label("ABI Version", as->osabi);
	}
	else
	{
		as->osabi_v[0] = '\0';
	}
	
	while (fread(tok, 1, 1, fp) && tok[0] == 0);
	fseek(fp, -1, SEEK_CUR);
	// do nothing
	
	advance(tok, 2, fp);
	
	
	spair_parser(bs->type, tok[0], elf_types, sizeof elf_types / sizeof(spr), "type");
	
	add_label("Type", bs->type);
	
	advance(tok, 2, fp);
	
	spair_parser(bs->arch, tok[0], elf_arches, sizeof elf_arches / sizeof(spr), "arch");
	
	add_label("Arch", bs->arch);
	
	advance(tok, 4, fp);
	
	// do nothing
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	address_parser(as->entry, tok, bits);
	
	add_label("Entry", as->entry);
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	address_parser(as->table, tok, bits);
	
	add_label("Table", as->table);
}
