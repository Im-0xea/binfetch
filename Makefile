PREFIX =/usr
CONFIG =/home/nik/.config/binfetch

CFLAGS = -Os -Wall
LDFLAGS = -lssl -lcrypto -flto

IB_HEADERS = $(wildcard *.h.ib)
IB_CFILES  = $(wildcard *.c.ib)
HEADERS = $(patsubst %.h.ib, build/%.h, $(IB_HEADERS))
OBJS = $(patsubst %.c.ib, build/%.o, $(IB_CFILES))
CFILES = $(patsubst %.c.ib, build/%.c, $(IB_CFILES))

build/%.h: %.h.ib build
	ib $< -o $@

build/%.c: %.c.ib build
	ib $< -o $@

build/%.o: build/%.c build
	gcc $< $(CFLAGS) -I build -c -o $@

all: $(HEADERS) $(CFILES) $(OBJS)
	gcc $(OBJS) $(LDFLAGS) -o binfetch
	strip binfetch

bootstrap:
	gcc ${CFILES} -o binfetch ${CFLAGS} ${LDFLAGS} -I build
	strip binfetch

clean:
	rm build/*.h build/*.o build/*.c

install:
	mkdir -p ${CONFIG}
	cp cfg/binfetch.cfg ${CONFIG}
	cp binfetch ${PREFIX}/bin/binfetch
