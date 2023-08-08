#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "pair.h"
#include "info.h"

#include "binfetch.h"
#include "config.h"
#include "ver.h"

int main(int argc, char **argv)
{
	const char * short_options = "hVc:";
	const struct option long_options[] = {
		{"help",    no_argument,       0, 'h'},
		{"version", no_argument,       0, 'V'},
		{"config",  required_argument, 0, 'c'},
		{0, 0, 0, 0},
	};

	char * config = NULL;
	do {
		const int opt = getopt_long(argc, argv, short_options, long_options, NULL);
		
		if (opt == -1) break;

		switch (opt) {
		default:
		case '?': // Unrecognized option
			return 1;
		case 'h':
			printf("Usage: binfetch [OPTION] [EXECUTABLES]\n\n"
			     " -h, --help    -> prints this\n"
			     " -V, --version -> prints version\n"
			     " -c, --config  -> set config file\n");
			return 0;
		case 'V':
			printf("Binfetch Version %.1f\n", VERSION);
			return 0;
		case 'c':
			config = optarg;
			break;
		}
	} while (0);

	if (optind >= argc) {
		set_color(red);
		printf("you did not provide a binary\n");
		set_blank();
		return 1;
	}

	parse_cfg(config);

	while (optind < argc)
		fetch(argv[optind++]);

	return 0;
}
