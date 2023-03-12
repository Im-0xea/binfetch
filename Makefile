PREFIX = /usr

CC = gcc
STRIP = strip

CFLAGS = -O0 -g -Wall
LDFLAGS = -flto

LIBS = -lcrypto -lssl -lz

PROGRAM = binfetch

IB_HEADERS = $(wildcard *.h.ib)
IB_CFILES  = $(wildcard *.c.ib)
HEADERS = $(patsubst %.h.ib, build/%.h, $(IB_HEADERS))
OBJS = $(patsubst %.c.ib, build/%.o, $(IB_CFILES))
CFILES = $(patsubst %.c.ib, build/%.c, $(IB_CFILES))

all: $(PROGRAM)

build/%.h: %.h.ib
	@echo " IB      $<"
	@ib $< -o $@

build/%.c: %.c.ib
	@echo " IB      $<"
	@ib $< -o $@

build/%.o: build/%.c
	@echo " CC      $<"
	@$(CC) $< $(CFLAGS) -I build -c -o $@ 

$(PROGRAM): $(HEADERS) $(CFILES) $(OBJS)
	@echo " LD      $(PROGRAM)"
	@$(CC) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROGRAM)
	@echo " STRIP   $<"
	#@$(STRIP) $(PROGRAM)

bootstrap: $(PROGRAM)

clean:
	@echo " CLEAN"
	@rm -f build/*.h build/*.o build/*.c $(PROGRAM)

install: $(PROGRAM)
	@echo " INSTALL $(PROGRAM)"
	@cp $(PROGRAM) ${PREFIX}/bin/
