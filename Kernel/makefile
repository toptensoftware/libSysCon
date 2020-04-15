LINKFLAGS=$(COMMONFLAGS) --code-loc 0x400 --data-loc 0x5280 --no-std-crt0

# Project config
INCLUDES 	:= $(wildcard *.h) ../libSysCon/libSysCon.h
INCLUDEPATH := 
LIBS	 	:= ../lib/libSysCon.lib ../Kernel.FatFS/bin/kernel.fatfs.sys.lib
ASMSOURCES  := ./crt0.s ./exports.s ./kernel.s
MAKEDEPS 	:= ../Kernel.FatFS ../libSysCon
OUTDIR		:= ./bin
YAZDFLAGS 	:= 
BINFILE     := kernel.sys
HEX2BINFLAGS := -s 0000
CLEAN		:= implib.s exports.s ./bin

# Default
.PHONY: all

all: makedeps exports.s binfile implib
	@echo > /dev/null

exports.s : exports.js
	node exports.js

include ../sdcc.mk
