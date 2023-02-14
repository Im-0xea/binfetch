#include "elf_arch.h"
#include "elf_type.h"
#include "elf_osabi.h"

typedef struct elf_info
{
	char version [128];
	char osabi   [128];
	char osabi_v [128];
	char entry   [128];
	char table   [128];
}
elf;

static void elf_parser(FILE * fp, base * bs)
{
	elf as;
	
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
	
	
	if (tok[2] == 0x01)
	{
		strcpy(as.version, "ELFv1");
	}
	else if (tok[2] == 0x00)
	{
		strcpy(as.version, "noncompliant");
	}
	
	
	spair_parser(as.osabi, tok[3], osabis, sizeof osabis / sizeof(spr), "abi");
	
	if (tok[4] != 0)
	{
		sprintf(as.osabi_v, "%d", tok[4]);
	}
	else
	{
		as.osabi_v[0] = '\0';
	}
	
	while (fread(tok, 1, 1, fp) && tok[0] == 0);
	fseek(fp, -1, SEEK_CUR);
	// do nothing
	
	advance(tok, 2, fp);
	
	
	spair_parser(bs->type, tok[0], elf_types, sizeof elf_types / sizeof(spr), "type");
	
	advance(tok, 2, fp);
	
	spair_parser(bs->arch, tok[0], elf_arches, sizeof elf_arches / sizeof(spr), "arch");
	
	advance(tok, 4, fp);
	
	// do nothing
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	address_parser(as.entry, tok, bits);
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	address_parser(as.table, tok, bits);
	
	print_label("Version");
	puts(as.version);
	print_label("OS ABI");
	puts(as.osabi);
	if (as.osabi_v[0] != '\0')
	{
		print_label("ABI Version");
		puts(as.osabi_v);
	}
	print_label("Entry");
	puts(as.entry);
	print_label("table");
	puts(as.table);
}
