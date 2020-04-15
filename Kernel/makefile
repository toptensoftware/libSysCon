LINKFLAGS=$(COMMONFLAGS) --code-loc 0x400 --data-loc 0x5280 --no-std-crt0

# Project config
INCLUDES 	:= $(wildcard *.h) ../libSysCon/libSysCon.h
INCLUDEPATH := 
LIBS	 	:= ../lib/libSysCon.lib ../Kernel.FatFS/bin/kernel.fatfs.sys.lib
ASMSOURCES  := ./crt0.s ./exports.s ./kernel.s
MAKEDEPS 	:= ../Kernel.FatFS ../libSysCon
OUTDIR		:= ./build
YAZDFLAGS 	:= --entry:0 --entry:0x66 --entry:0x100 --entry:0x103 --entry:0x106
BINFILE     := kernel.sys
HEX2BINFLAGS := -s 0000 -l 8000
CLEAN		:= implib.s exports.s ./bin

# Default
all: makedeps exports.s binfile implib ./bin/kernel.sys done

# Combine kernel.base.sys and kernel.fatfs.sys
./bin/kernel.sys: $(OUTDIR)/kernel.sys
	@echo Combining overlays...
	@mkdir -p ./bin
	@cat $(OUTDIR)/kernel.sys ../Kernel.FatFS/bin/kernel.fatfs.sys > ./bin/kernel.sys
	@cp $(OUTDIR)/kernel.sys.lib ./bin/kernel.sys.lib
	@cp $(OUTDIR)/Kernel.map ~/sf_downloads
	@cp ../Kernel.FatFS/build/Kernel.FatFS.map ~/sf_downloads

exports.s : exports.js
	node exports.js

include ../sdcc.mk
