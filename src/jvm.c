#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"
#include "pair.h"

#include "binfetch.h"
#include "bin_op.h"

void jvm_parser(FILE * fp, uint32_t ver)
{
	sprintf(ibuffer[buffer_pos], "%u", ver);
	add_label("Version", ibuffer[buffer_pos++]);
}
