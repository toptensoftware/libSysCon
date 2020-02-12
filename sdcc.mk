# ------------------------- Tool Chain -------------------------

LIBSYSCON := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
LIBSYSCON := $(LIBSYSCON:/=)

SDCCDIR ?= /opt/sdcc
SDCC ?= sdcc-filter $(SDCCDIR)/bin/sdcc
SDAR ?= $(SDCCDIR)/bin/sdar
SDASZ80 ?= $(SDCCDIR)/bin/sdasz80
HEX2BIN ?= $(LIBSYSCON)/tools/hex2bin
YAZD ?= yazd



# ------------------------- Project Options -------------------------


PROJNAME ?= $(notdir $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))))

BINFILE ?= $(PROJNAME).bin

CSOURCES  ?= $(wildcard *.c)
ASMSOURCES  ?= $(wildcard *.s)
INCLUDES ?= $(wildcard *.h)

COMMONFLAGS ?= -mz80 --stack-auto
CFLAGS ?= --std-c99 --disable-warning 85 --disable-warning 110 --disable-warning 126
ASMFLAGS ?= 
YAZDFLAGS ?= --entry:0

INTDIR ?= ./build
OUTDIR ?= $(INTDIR)



# ------------------------- Object Files -------------------------


COBJS := $(CSOURCES:%.c=$(INTDIR)/%.rel)
ASMOBJS := $(ASMSOURCES:%.s=$(INTDIR)/%.rel)



# ------------------------- Clean  -------------------------

# Optional files/folders to remove
CLEAN ?= 

# Clean target
clean:
	@rm -rf $(INTDIR)
	@rm -f $(OUTDIR)/$(PROJNAME).lib
	@rm -f $(OUTDIR)/$(BINFILE)
	@for p in $(CLEAN); do rm -rf $$p; done



# ------------------------- Build a Binary -------------------------

.PHONY: binfile

binfile: $(OUTDIR)/$(BINFILE)

# Link
$(OUTDIR)/$(BINFILE): $(ASMOBJS) $(COBJS) $(LIBS)
	@echo Linking $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@$(SDCC) $(LINKFLAGS) $^ -o $(INTDIR)/$(PROJNAME).ihx
	@$(HEX2BIN) -b -s 0000 -e bin.tmp $(INTDIR)/$(PROJNAME).ihx
	@mv $(INTDIR)/$(PROJNAME).bin.tmp $(OUTDIR)/$(BINFILE)



# ------------------------- Build Dependencies -------------------------

.PHONY: makedeps

makedeps:
	@for p in $(MAKEDEPS); do echo Making $$p; $(MAKE) --no-print-directory -C $$p; echo ; done




# ------------------------- Disassemble Binary -------------------------


yazd: $(OUTDIR)/$(BINFILE).lst

# Link
$(OUTDIR)/$(BINFILE).lst: $(OUTDIR)/$(BINFILE)
	@echo Disassembling $(notdir $<)...
	@$(YAZD) $< $(YAZDFLAGS) --lst > $@
	@echo done.



# ------------------------- Build a Library -------------------------

.PHONY : lib

lib: $(OUTDIR)/$(PROJNAME).lib

# Build lib
$(OUTDIR)/$(PROJNAME).lib: $(COBJS)	$(ASMOBJS)
	@echo Creating library $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@rm -f $@
	@$(SDAR) -rc $@ $(COBJS) $(ASMOBJS)



# ------------------------- Compile C Files -------------------------

# Compile
$(INTDIR)/%.rel : ./%.c $(INCLUDES)
	@echo Compiling $(notdir $<)...
	@mkdir -p $(INTDIR)
	@$(SDCC) $(COMMONFLAGS) $(CFLAGS) $(addprefix -I ,$(INCLUDEPATH)) -c $< -o $@



# ------------------------- Assemble S Files -------------------------

# Assemble
$(INTDIR)/%.rel: ./%.s
	@echo Assembling $(notdir $<)...
	@mkdir -p $(INTDIR)
	@$(SDASZ80) $(ASMFLAGS) -l -o $@ $<


