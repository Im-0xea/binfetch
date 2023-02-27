#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "info.h"
#include "pair.h"
#include "bin_op.h"

extern char ibuffer[128][128];
extern int  buffer_pos;

void sh_parser(FILE * fp)
{
	fseek(fp, -2, SEEK_CUR);
	fgets(ibuffer[buffer_pos], 255, fp);
	ibuffer[buffer_pos][strlen(ibuffer[buffer_pos]) - 1] = '\0';
	add_label("Interpreter", ibuffer[buffer_pos++]);
	
	return;
}
