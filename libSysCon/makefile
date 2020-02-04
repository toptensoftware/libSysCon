COMMONFLAGS=-mz80 --stack-auto
CFLAGS=$(COMMONFLAGS) --std-c99 --disable-warning 85 --disable-warning 110 --disable-warning 126
LINKFLAGS=$(COMMONFLAGS) --code-loc 0x110 --data-loc 0xF400 --no-std-crt0
INTDIR=./build
OUTDIR=../lib

SDCCDIR=/opt/sdcc
SDCC=$(SDCCDIR)/bin/sdcc
SDASZ80=$(SDCCDIR)/bin/sdasz80
HEX2BIN=../../tools/hex2bin
YAZD=yazd

PROJNAME := libSysCon
SOURCES  := 
INCLUDES := 
PROJOBJS := $(SOURCES:%.c=$(INTDIR)/%.rel)
OBJECTS  := $(INTDIR)/crt0.rel $(PROJOBJS)

all: $(OUTDIR)/crt0.rel

# Compile
$(PROJOBJS): $(INTDIR)/%.rel : ./%.c $(INCLUDES)
	$(SDCC) $(CFLAGS) -c $< -o $@

# Runtime start
$(OUTDIR)/crt0.rel: crt0.s
	$(SDASZ80) -l -o $@ $<

# Directories
$(INTDIR):
	mkdir -p $(INTDIR)

# Clean
clean:
	rm -f $(OUTDIR)/$(PROJNAME).*
	rm -f $(OUTDIR)/crt0.rel
	rm -rf $(INTDIR)

