static void pe_parser(FILE * fp)
{
	unsigned short tok;
	
	advance(&tok, 2, fp);
	
	print_label("Arch");
	switch (tok)
	{
		case 0x014C:
			printf("x86\n");
			break;
		case 0x8664:
			printf("x86_64\n");
			break;
		case 0x01f0:
			printf("arm\n");
			break;
		case 0x01c0:
			printf("arm thumb-2\n");
			break;
	}
}
