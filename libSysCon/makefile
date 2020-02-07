COMMONFLAGS=-mz80 --stack-auto
CFLAGS=$(COMMONFLAGS) --std-c99 --disable-warning 85 --disable-warning 110 --disable-warning 126
LINKFLAGS=$(COMMONFLAGS) --code-loc 0x110 --data-loc 0xF400 --no-std-crt0
INTDIR=./build
OUTDIR=../lib

ifeq ($(OS),Windows_NT)
SDCC=sdcc
SDAR=sdar
SDASZ80=sdasz80
else
SDCCDIR=/opt/sdcc
SDCC=sdcc-filter $(SDCCDIR)/bin/sdcc
SDAR=$(SDCCDIR)/bin/sdar
SDASZ80=$(SDCCDIR)/bin/sdasz80
endif

HEX2BIN=../../tools/hex2bin
YAZD=yazd

PROJNAME := libSysCon
SOURCES  := $(wildcard *.c)
INCLUDES := $(wildcard *.h)
PROJOBJS := $(SOURCES:%.c=$(INTDIR)/%.rel)
INCLUDEPATH := ../libFatFS

all: $(OUTDIR)/crt0.rel $(OUTDIR)/$(PROJNAME).lib

# Build lib
$(OUTDIR)/$(PROJNAME).lib: $(PROJOBJS)	
	@echo Creating library $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@rm -f $@
	@$(SDAR) -rc $@ $(PROJOBJS)

# Compile
$(PROJOBJS): $(INTDIR)/%.rel : ./%.c $(INCLUDES)
	@echo Compiling $(notdir $<)...
	@mkdir -p $(INTDIR)
	@$(SDCC) $(CFLAGS) $(addprefix -I ,$(INCLUDEPATH)) -c $< -o $@

# Runtime start 
$(OUTDIR)/crt0.rel: crt0.s
	@mkdir -p $(OUTDIR)
	@$(SDASZ80) -l -o $@ $<

# Clean
clean:
	@rm -f $(OUTDIR)/$(PROJNAME).*
	@rm -f $(OUTDIR)/crt0.rel
	@rm -rf $(INTDIR)

