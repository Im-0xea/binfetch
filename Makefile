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

SRC = $(wildcard src/*.c)

OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

PRG = binfetch
B = build
S = src

all: $(B)/$(PRG)

$(B)/%.o: $(S)/%.c
	@echo "  CC      $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(B)/$(PRG): $(OBJ)
	@echo "  LD      $(PRG)"
	@$(LD) $(LDFLAGS) $(LIBS) -o $(PRG) $(OBJ)

$(B):
	mkdir -p build

install:
	@echo "  INSTALL $(PRG)"
	@mkdir -p $(DESTDIR)/$(PREFIX)/bin
	@cp $(PRG) $(DESTDIR)/$(PREFIX)/bin/

test:
	@echo "  TESTING"
	@./$(PRG) $(PRG)

clean:
	@echo "  CLEAN"  
	@rm -f $(OBJ) $(PRG)
