COMMONFLAGS=-mz80
CFLAGS=$(COMMONFLAGS) --opt-code-size --disable-warning 126 --disable-warning 110
INTDIR=build
OUTDIR=../lib
SRCDIR=.
ifeq ($(OS),Windows_NT)
SDCC=sdcc
SDAR=sdar
else
SDCCDIR=/opt/sdcc
SDCC=$(SDCCDIR)/bin/sdcc
SDAR=$(SDCCDIR)/bin/sdar
endif

PROJNAME := libFatFS
SOURCES  := $(wildcard *.c)
INCLUDES := ff.h ffpriv.h ffconf.h
OBJS  	 := $(SOURCES:%.c=$(INTDIR)/%.rel)


# Default
all: $(OUTDIR)/$(PROJNAME).lib

# Clean
clean:
	@rm -rf $(INTDIR)
	@rm -rf $(OUTDIR)/$(PROJNAME).lib

# Rebuild
rebuild: clean all

# Compile
$(OBJS): $(INTDIR)/%.rel : $(SRCDIR)/%.c $(INCLUDES)
	@echo Compiling $(notdir $<)...
	@mkdir -p $(INTDIR)
	@$(SDCC) $(CFLAGS) $(FFOPTS) -c $< -o $@

# Link
$(OUTDIR)/$(PROJNAME).lib:  $(OBJS) 
	@echo Creating library $(notdir $@)...
	@rm -f $@
	@mkdir -p $(OUTDIR)
	@$(SDAR) -rc $@ $(OBJS)
	@echo Finished.

