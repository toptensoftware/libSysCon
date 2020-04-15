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
IMPLIB ?= $(PROJNAME).lib

CSOURCES  ?= $(wildcard *.c)
ASMSOURCES  ?= $(wildcard *.s)
INCLUDES ?= $(wildcard *.h)

COMMONFLAGS ?= -mz80 --stack-auto
CFLAGS ?= --std-c99 --disable-warning 85 --disable-warning 110 --disable-warning 126
ASMFLAGS ?= 
YAZDFLAGS ?= --entry:0
HEX2BINFLAGS ?= -s 0000

INTDIR ?= ./build
OUTDIR ?= $(INTDIR)



# ------------------------- Object Files -------------------------


COBJS := $(CSOURCES:%.c=$(INTDIR)/%.rel)
ASMOBJS := $(ASMSOURCES:%.s=$(INTDIR)/%.rel)



# ------------------------- Clean  -------------------------

.PHONY: clean

# Optional files/folders to remove
CLEAN ?= 

# Clean target
clean:
	@rm -rf $(INTDIR)
	@rm -f $(OUTDIR)/$(PROJNAME).lib
	@rm -f $(OUTDIR)/$(BINFILE)
	@for p in $(CLEAN); do rm -rf $$p; done


# ------------------------- Clean All  -------------------------

.PHONY: cleanall

# Clean target
cleanall: clean cleandeps



# ------------------------- Build a Binary -------------------------

.PHONY: binfile

binfile: $(OUTDIR)/$(BINFILE)

# Link
$(OUTDIR)/$(BINFILE): $(ASMOBJS) $(COBJS) $(LIBS)
	@echo Linking $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@$(SDCC) $(LINKFLAGS) $^ -o $(INTDIR)/$(PROJNAME).ihx
	@$(HEX2BIN) -b $(HEX2BINFLAGS) -e bin.tmp $(INTDIR)/$(PROJNAME).ihx > /dev/null
	@mv $(INTDIR)/$(PROJNAME).bin.tmp $(OUTDIR)/$(BINFILE)



# ------------------------- Build a Library -------------------------

.PHONY : implib

implib: $(OUTDIR)/$(BINFILE).lib

# Build lib
$(OUTDIR)/$(BINFILE).lib: implib.s
	@echo Creating import library $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@rm -f $@
	@$(SDASZ80) $(ASMFLAGS) -l -o $(INTDIR)/implib.rel $<
	@$(SDAR) -rc $@ $(INTDIR)/implib.rel


# ------------------------- Build Dependencies -------------------------

.PHONY: makedeps

makedeps:
	@for p in $(MAKEDEPS); do $(MAKE) --no-print-directory -C $$p; done




# ------------------------- Build Dependencies -------------------------

.PHONY: cleandeps

cleandeps:
	@for p in $(MAKEDEPS); do $(MAKE) --no-print-directory -C $$p cleanall ; done




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


