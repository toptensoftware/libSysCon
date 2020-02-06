COMMONFLAGS=-mz80
CFLAGS=$(COMMONFLAGS) --opt-code-size --disable-warning 126 --disable-warning 110
INTDIR=./build
OUTDIR=../lib
SRCDIR=.
SDCCDIR=/opt/sdcc
SDCC=$(SDCCDIR)/bin/sdcc
SDAR=$(SDCCDIR)/bin/sdar

PROJNAME := libFatFSBoot
SOURCES  := $(SRCDIR)/ff.c
INCLUDES := $(SRCDIR)/ff.h
OBJS  	 := $(SOURCES:%.c=$(INTDIR)/%.rel)


# Default
all: $(OUTDIR)/$(PROJNAME).lib

# Clean
clean:
	@rm -rf $(INTDIR)
	@rm -rf $(OUTDIR)/$(PROJNAME).lib

# Compile
$(OBJS): $(INTDIR)/%.rel : $(SRCDIR)/%.c $(INCLUDES)
	@echo Compiling $(notdir $<)...
	@mkdir -p $(INTDIR)
	@$(SDCC) $(CFLAGS) $(FFOPTS) -c $< -o $@

# Link
$(OUTDIR)/$(PROJNAME).lib: $(OBJS) 
	@echo Creating library $(notdir $@)...
	@mkdir -p $(OUTDIR)
	@rm -f $@
	@$(SDAR) -rc $@ $(OBJS)

