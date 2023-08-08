#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"

void sh_parser(FILE * fp)
{
	fseek(fp, -2, SEEK_CUR);
	if (fgets(ibuffer[buffer_pos], 255, fp))
		ibuffer[buffer_pos][strlen(ibuffer[buffer_pos]) - 1] = '\0';
	else
		strcpy(ibuffer[buffer_pos], "failed to parse");
	add_label("Interpreter", ibuffer[buffer_pos++]);

	return;
}
