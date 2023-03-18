DESTDIR= /
PREFIX = /usr/local

SRC = ./src
DEST = ./build

IB = ib
CC ?= gcc
LD = $(CC)
STRIP = strip

CFLAGS ?= -Os -Wall
CPPFLAGS = -DPREFIX=\"$(PREFIX)\"
LDFLAGS ?= -flto -lm

LIBS = -lz

PROGRAM = binfetch

IB_HEADERS = $(wildcard $(SRC)/*.h.ib)
IB_CFILES  = $(wildcard $(SRC)/*.c.ib)
HEADERS = $(patsubst $(SRC)/%.h.ib, $(DEST)/%.h, $(IB_HEADERS))
OBJS = $(patsubst $(SRC)/%.c.ib, $(DEST)/%.o, $(IB_CFILES))
CFILES = $(patsubst $(SRC)/%.c.ib, $(DEST)/%.c, $(IB_CFILES))

all: $(DEST) check_ib show_flags $(PROGRAM)

$(DEST)/%.h: $(SRC)/%.h.ib
	@echo " IB      $<"
	@$(IB) $< -o $@

$(DEST)/%.c: $(SRC)/%.c.ib
	@echo " IB      $<"
	@$(IB) $< -o $@

$(DEST)/%.o: $(DEST)/%.c
	@echo " CC      $<"
	@$(CC) $< $(CFLAGS) $(CPPFLAGS) -I build -c -o $@ 

$(PROGRAM): $(HEADERS) $(CFILES) $(OBJS)
	@echo " LD      $(PROGRAM)"
	@$(LD) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROGRAM)
	@echo " STRIP   $(PROGRAM)"
	@$(STRIP) $(PROGRAM)

bootstrap: $(DEST) build_ib $(PROGRAM)

$(DEST):
	mkdir -p $(DEST)

build_ib:
	$(CC) $(SRC)/ib/ib.c -o ib_strap
	$(eval IB = ./ib_strap)

show_flags:
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

check_ib:
	@which ib > /dev/null 2>&1; \
	if [ $$? -ne 0 ]; then \
		echo "ib is not installed or not executable, either install ib from my repo, or run 'make bootstrap'"; \
		exit 1; \
	fi

clean:
	@echo " CLEAN"
	@rm -f $(DEST)/*.h $(DEST)/*.o $(DEST)/*.c $(PROGRAM)

install: $(PROGRAM)
	@echo " INSTALL $(PROGRAM)"
	@mkdir -p $(DESTDIR)/${PREFIX}/bin/
	@cp $(PROGRAM) $(DESTDIR)/${PREFIX}/bin/
	@echo " INSTALL *.cfg"
	@mkdir -p $(DESTDIR)/${PREFIX}/share/binfetch
	@cp cfg/* $(DESTDIR)/$(PREFIX)/share/binfetch
