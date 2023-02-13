PREFIX =/usr

CFLAGS = -Os -g -Wall
LDFLAGS = -lssl -lcrypto

HEADERS = *.h.ib

all:
	ib ${HEADERS}
	ib -in binfetch.c.ib --flags "${CFLAGS} ${LDFLAGS}"

bootstrap:
	gcc binfetch.c -o binfetch ${CFLAGS} ${LDFLAGS}

install:
	cp binfetch ${PREFIX}/bin/binfetch
