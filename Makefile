PREFIX =/usr
CONFIG =/home/nik/.config/binfetch

CFLAGS = -Os -Wall
LDFLAGS = -lssl -lcrypto -flto

HEADERS = *.h.ib
IBCFILES  = binfetch.c.ib elf.c.ib mach.c.ib pe.c.ib pair.c.ib crypto.c.ib
CFILES  = binfetch.c elf.c mach.c pe.c pair.c crypto.c

all:
	ib ${HEADERS}
	ib -in ${IBCFILES} --flags "${CFLAGS} ${LDFLAGS}" -o binfetch

bootstrap:
	gcc ${CFILES} -o binfetch ${CFLAGS} ${LDFLAGS}

install:
	mkdir -p ${CONFIG}
	cp cfg/binfetch.cfg ${CONFIG}
	cp binfetch ${PREFIX}/bin/binfetch
