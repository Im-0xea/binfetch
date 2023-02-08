PREFIX=/usr

all:
	ib -in binfetch.c.ib

bootstrap:
	gcc binfetch.c -o binfetch

install:
	cp binfetch ${PREFIX}/bin/binfetch
