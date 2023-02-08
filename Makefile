PREFIX=/usr

all:
	ib -in pair.h.ib
	ib -in binfetch.c.ib

bootstrap:
	gcc binfetch.c -o binfetch

install:
	cp binfetch ${PREFIX}/bin/binfetch
