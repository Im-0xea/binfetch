typedef unsigned char byte;
typedef unsigned int fbyte;

typedef struct small_pair
{
	byte key;
	char str[64];
}
spr;

typedef struct big_pair
{
	fbyte key;
	char str[64];
}
bpr;

extern void spair_parser(char * out, const byte val, const spr * prs, const size_t size, const char * type);
extern void bpair_parser(char * out, const fbyte val, const bpr * prs, const size_t size, const char * type);
