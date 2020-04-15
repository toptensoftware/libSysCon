LINKFLAGS=$(COMMONFLAGS) --code-loc 0x8100 --data-loc 0xF800 --no-std-crt0

# Project config
INCLUDES 	:= $(wildcard *.h) 
INCLUDEPATH := ../libFatFS/
LIBS	 	:= ../lib/libFatFS.lib
ASMSOURCES  := ./crt0.s ./exports.s
MAKEDEPS 	:= ../libFatFS
OUTDIR		:= ./bin
BINFILE     := kernel.fatfs.sys
HEX2BINFLAGS := -s 8000 -l 8000
CLEAN		:= implib.s exports.s ./bin
YAZDFLAGS 	:= \
	--addr:0x8000 \
	--entry:0x8000 \
	--entry:0x8003 \
	--entry:0x8006 \
	--entry:0x8009 \
	--entry:0x800C \
	--entry:0x800F \
	--entry:0x8012 \
	--entry:0x8015 \
	--entry:0x8018 \
	--entry:0x801B \
	--entry:0x801E \
	--entry:0x801E \
	--entry:0x8021 \
	--entry:0x8024 \
	--entry:0x8027 \
	--entry:0x802A \
	--entry:0x802D \
	--entry:0x8030 \
	--entry:0x8033 \
	--entry:0x8036 \
	--entry:0x8039

# Default
all: makedeps exports.s binfile implib done

exports.s : exports.js
	node exports.js

include ../sdcc.mk

