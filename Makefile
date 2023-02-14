PREFIX =/usr
CONFIG =/home/nik/.config/binfetch

CFLAGS = -Os -Wall -g
LDFLAGS = -lssl -lcrypto

HEADERS = *.h.ib

all:
	ib ${HEADERS}
	ib -in binfetch.c.ib --flags "${CFLAGS} ${LDFLAGS}"

bootstrap:
	gcc binfetch.c -o binfetch ${CFLAGS} ${LDFLAGS}

install:
	mkdir -p ${CONFIG}
	cp cfg/binfetch.cfg ${CONFIG}
	cp binfetch ${PREFIX}/bin/binfetch
