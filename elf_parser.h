static void elf_parser(FILE * fp)
{
	byte tok[max_tok];
	
	advance(tok, 5, fp);
	
	int bits = 0 ;// 32 or 64
	
	print_label("Class");
	
	if (tok[0] == 0x01)	
	{
		bits = 1;
		printf("32 bit\n");
	}
	else if (tok[0] == 0x02)
	{
		bits = 2;
		printf("64 bit\n");
	}
	else
	{
		printf("unknown");
	}
	
	print_label("Endianness");
	
	if (tok[1] == 0x01)
	{
		printf("little endian\n");
	}
	else if (tok[1] == 0x02)
	{
		printf("big endian\n");
	}
	else
	{
		printf("unknown endian\n");
	}
	
	print_label("Version");
	
	if (tok[2] == 0x01)
	{
		printf("ELFv1\n");
	}
	else if (tok[2] == 0x00)
	{
		printf("noncompliant\n");
	}
	
	print_label("OS ABI");
	
	spair_parser(tok[3], osabis, sizeof osabis / sizeof(spr), "abi");
	
	if (tok[4] != 0)
	{
		print_label("ABI Version");
		
		printf("%d\n", tok[4]);
	}
	
	while (fread(tok, 1, 1, fp) && tok[0] == 0);
	fseek(fp, -1, SEEK_CUR);
	// do nothing
	
	advance(tok, 2, fp);
	
	print_label("Type");
	
	spair_parser(tok[0], elf_types, sizeof elf_types / sizeof(spr), "type");
	
	advance(tok, 2, fp);
	
	print_label("Arch");
	
	spair_parser(tok[0], elf_arches, sizeof elf_arches / sizeof(spr), "arch");
	
	advance(tok, 4, fp);
	
	// do nothing
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	print_label("Entry");
	
	printf("0x");
	
	{
		char out[128] = "";
		signed long b = bits == 1 ? 4 : 8;
		while (--b >= 0)
		{
			if (!tok[b]) continue;
			char bit[3];
			sprintf(bit, "%02x", tok[b]);
			strcat(out,bit);
		}
		
		printf("%s\n", out);
	}
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	print_label("Table");
	printf("0x");
	
	{
		char out[128] = "";
		signed long b = bits == 1 ? 4 : 8;
		while (--b >= 0)
		{
			if (!tok[b]) continue;
			char bit[3];
			sprintf(bit, "%02x", tok[b]);
			strcat(out,bit);
		}
		
		printf("%s\n", out);
	}
}
