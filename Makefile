PREFIX=/usr

CFLAGS= -Os

all:
	ib pair.h.ib
	ib arch.h.ib
	ib osabi.h.ib
	ib -in binfetch.c.ib --flags "${CFLAGS}"

bootstrap:
	gcc binfetch.c -o binfetch ${CFLAGS}

install:
	cp binfetch ${PREFIX}/bin/binfetch
