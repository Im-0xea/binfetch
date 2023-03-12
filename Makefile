PREFIX = /usr
CONFIG = /usr/share/binfetch

SRC = ./src
DEST = ./build

IB = ib
CC = gcc
LD = gcc
STRIP = strip

CFLAGS = -Os -Wall
LDFLAGS = -flto

LIBS = -lz

PROGRAM = binfetch

IB_HEADERS = $(wildcard $(SRC)/*.h.ib)
IB_CFILES  = $(wildcard $(SRC)/*.c.ib)
HEADERS = $(patsubst $(SRC)/%.h.ib, $(DEST)/%.h, $(IB_HEADERS))
OBJS = $(patsubst $(SRC)/%.c.ib, $(DEST)/%.o, $(IB_CFILES))
CFILES = $(patsubst $(SRC)/%.c.ib, $(DEST)/%.c, $(IB_CFILES))

all: check_ib $(PROGRAM)

$(DEST)/%.h: $(SRC)/%.h.ib
	@echo " IB      $<"
	@$(IB) $< -o $@

$(DEST)/%.c: $(SRC)/%.c.ib
	@echo " IB      $<"
	@$(IB) $< -o $@

$(DEST)/%.o: $(DEST)/%.c
	@echo " CC      $<"
	@$(CC) $< $(CFLAGS) -I build -c -o $@ 

$(PROGRAM): $(HEADERS) $(CFILES) $(OBJS)
	@echo " LD      $(PROGRAM)"
	@$(LD) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROGRAM)
	@echo " STRIP   $(PROGRAM)"
	@$(STRIP) $(PROGRAM)

bootstrap: build_ib $(PROGRAM)

build_ib:
	gcc $(SRC)/ib/ib.c -o ib_strap
	$(eval IB = ./ib_strap)

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
	@mkdir -p ${PREFIX}/bin/
	@cp $(PROGRAM) ${PREFIX}/bin/
	@echo " INSTALL *.cfg"
	@mkdir -p ${CONFIG}/
	@cp cfg/* $(CONFIG)/
