# Makefile for Lax 6.0
# Time-stamp: <2026-1-01 17:19:40 Mavox-ID>

# Unix-systems
CC = gcc -pedantic
PIC = -fpic
Osuf=o
SOsuf=so
LIBsuf=a
EXE_EXT=
LIBPREFIX=lib
OUT = -o $@
RM= -rm -f
AR= ar crs

# Linux
LD = gcc
LDFLAGS = -shared -s -flto=auto
DEBUG = -Os -fno-stack-protector -fno-ident -fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections -fomit-frame-pointer -flto=auto
SYS_LIBS = -ldl -lm

FEATURES = -DUSE_MATH=1 -DUSE_ASCII_NAMES=1

OBJS = Lax.$(Osuf) linenoise.$(Osuf)

LIBTARGET = $(LIBPREFIX)Lax.$(SOsuf)
STATICLIBTARGET = $(LIBPREFIX)Lax.$(LIBsuf)

# UPX
UPX = upx
UPXFLAGS = --best --lzma -qq

PREFIX = /usr/local
BINDIR = $(PREFIX)/Lax

all: $(LIBTARGET) $(STATICLIBTARGET) Lax$(EXE_EXT)

Lax$(EXE_EXT): $(OBJS)
	$(CC) $(DEBUG) -o $@ $(OBJS) $(SYS_LIBS)
	strip --strip-all $@
	$(UPX) $(UPXFLAGS) $@

%.$(Osuf): %.c
	$(CC) $(PIC) -I. -c $(DEBUG) $(FEATURES) $(DL_FLAGS) $<

$(LIBTARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OUT) $(OBJS) $(SYS_LIBS)

$(STATICLIBTARGET): $(OBJS)
	$(AR) $@ $(OBJS)

$(OBJS): Lax.h Lax-cell.h com.h
linenoise.$(Osuf): linenoise.h

install: Lax$(EXE_EXT)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 Lax$(EXE_EXT) $(DESTDIR)$(BINDIR)/Lax

uninstall:
	$(RM) $(DESTDIR)$(BINDIR)/Lax

help:
	@echo "Targets:"
	@echo "  make            - Build binary"
	@echo "  make clean      - Remove build files"
	@echo "  make install    - Install binary to system"
	@echo "  make uninstall  - Uninstall binary for system"
	@echo "  make help       - Show this help"

clean:
	$(RM) $(OBJS) $(LIBTARGET) $(STATICLIBTARGET) Lax$(EXE_EXT)
	$(RM) Lax.ilk Lax.map Lax.pdb Lax.exp
	$(RM) Lax.ilk Lax.map Lax.pdb Lax.lib Lax.exp
	$(RM) *~ .lax_history

TAGS_SRCS = Lax.h Lax.c linenoise.h linenoise.c

tags: TAGS
TAGS: $(TAGS_SRCS)
	etags $(TAGS_SRCS)
