PREFIX = /usr
CONFIG = /tmp

CFLAGS = -Os -Wall
LDFLAGS = -lssl -lcrypto -flto

IB_HEADERS = $(wildcard *.h.ib)
IB_CFILES  = $(wildcard *.c.ib)
HEADERS = $(patsubst %.h.ib, build/%.h, $(IB_HEADERS))
OBJS = $(patsubst %.c.ib, build/%.o, $(IB_CFILES))
CFILES = $(patsubst %.c.ib, build/%.c, $(IB_CFILES))

all: clean binfetch
	strip binfetch

build/%.h: %.h.ib
	ib $< -o $@

build/%.c: %.c.ib
	ib $< -o $@

build/%.o: build/%.c
	gcc $< $(CFLAGS) -I build -D CONFIG="$(CONFIG)" -c -o $@ 

binfetch: $(HEADERS) $(CFILES) $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o binfetch

bootstrap: binfetch
	strip binfetch

clean:
	rm -f build/*.h build/*.o build/*.c binfetch

install:
	mkdir -p ${CONFIG}
	cp cfg/binfetch.cfg ${CONFIG}
	cp binfetch ${PREFIX}/bin/binfetch
