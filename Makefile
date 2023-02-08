PREFIX=/usr

CFLAGS= -Os -Wall

all:
	ib pair.h.ib
	ib elf_arch.h.ib
	ib mach_arch.h.ib
	ib elf_osabi.h.ib
	ib -in binfetch.c.ib --flags "${CFLAGS}"

bootstrap:
	gcc binfetch.c -o binfetch ${CFLAGS}

install:
	cp binfetch ${PREFIX}/bin/binfetch
