CC := cc
LD := cc

DESTDIR = /
PREFIX = /usr/local

CFLAGS := -Os -Wall
LDFLAGS := -flto

CPPFLAGS := -Iinclude \
	$(shell pkg-config --cflags zlib) \
	$(shell pkg-config --cflags libelf)

LIBS := \
	-lm \
	$(shell pkg-config --libs zlib) \
	$(shell pkg-config --libs libelf)

.SUFFIXES: .c .o

OBJS = \
	src/args.o \
	src/binfetch.o \
	src/bin_op.o \
	src/color.o \
	src/config.o \
	src/crypto.o \
	src/elf_parser.o \
	src/info.o \
	src/jvm.o \
	src/mach.o \
	src/mz.o \
	src/pair.o \
	src/pe.o \
	src/sh.o \
	src/uf2.o

PRG = binfetch

all: $(PRG)

$(OUT):
	mkdir -p $(OUT)

.c.o:
	@echo "  CC      $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(PRG): $(OBJS)
	@echo "  LD      $(PRG)"
	@$(LD) $(LDFLAGS) $(LIBS) -o $(PRG) $(OBJS)

install:
	@echo "  INSTALL $(PRG)"
	@mkdir -p $(DESTDIR)/$(PREFIX)/bin
	@cp $(PRG) $(DESTDIR)/$(PREFIX)/bin/

clean:
	@echo "  CLEAN"  
	@rm -f $(OBJS) $(PRG)
