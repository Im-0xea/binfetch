static void sha512_art(FILE * fp)
{
	int i, bytes;
	SHA512_CTX ctx;
	unsigned char buf[1024], data[64];
	
	SHA512_Init (&ctx);
	while ((bytes = fread (buf, 1, 1024, fp)) != 0)
	{
		SHA512_Update (&ctx, buf, bytes);
	}
	
	SHA512_Final(data, &ctx);
	i = 0;
	for(i = 0; i < 64; i++)
	{
		sprintf(&ascii_art[i / 8][i % 8 * 2],"%02x", data[i]);
		int len = strlen(ascii_art[i / 8]);
		if (len > max_line)
		{
			max_line = len;
		}
	}
	i = 0;
	
	rewind(fp);
}

static void sha256_art(FILE * fp)
{
	int i, bytes;
	SHA256_CTX ctx;
	unsigned char buf[1024], data[32];
	
	SHA256_Init (&ctx);
	while ((bytes = fread (buf, 1, 1024, fp)) != 0)
	{
		SHA256_Update (&ctx, buf, bytes);
	}
	
	SHA256_Final(data, &ctx);
	i = 0;
	for(i = 0; i < 32; i++)
	{
		sprintf(&ascii_art[i / 8][i % 4 * 2],"%02x", data[i]);
		int len = strlen(ascii_art[i / 4]);
		if (len > max_line)
		{
			max_line = len;
		}
	}
	i = 0;
	
	rewind(fp);
}
