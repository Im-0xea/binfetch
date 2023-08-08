#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "info.h"

#include "binfetch.h"

void add_label(char * name, char * out)
{
	strcpy(labels[label_count].key, name);
	labels[label_count].out = out;
	labels[label_count++].used = false;
}
