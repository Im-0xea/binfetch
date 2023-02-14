typedef struct infob
{
	char name  [128];
	char header[128];
	char class [128];
	char endian[128];
	char arch  [128];
	char type  [128];
	char size  [128];
}
base;

typedef struct elf_info
{
	char version [128];
	char osabi   [128];
	char osabi_v [128];
	char entry   [128];
	char table   [128];
}
elf;


typedef struct label_s
{
	char   key[128];
	char * out;
	bool   used;
}
label;
