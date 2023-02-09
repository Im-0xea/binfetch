PREFIX =/usr

CFLAGS = -Os -Wall
LDFLAGS = -lssl -lcrypto

all:
	ib pair.h.ib
	ib elf_arch.h.ib
	ib mach_arch.h.ib
	ib elf_osabi.h.ib
	ib -in binfetch.c.ib --flags "${CFLAGS} ${LDFLAGS}"

bootstrap:
	gcc binfetch.c -o binfetch ${CFLAGS} ${LDFLAGS}

install:
	cp binfetch ${PREFIX}/bin/binfetch
