LINKFLAGS=$(COMMONFLAGS) --code-loc 0x8100 --data-loc 0xF800 --no-std-crt0

# Project config
INCLUDES 	:= $(wildcard *.h) 
INCLUDEPATH := ../libFatFS/
LIBS	 	:= ../lib/libFatFS.lib
ASMSOURCES  := ./crt0.s ./exports.s
MAKEDEPS 	:= ../libFatFS
OUTDIR		:= ./bin
BINFILE     := kernel.fatfs.sys
HEX2BINFLAGS := -s 8000
CLEAN		:= implib.s exports.s ./bin
YAZDFLAGS 	:= 

# Default
all: makedeps exports.s binfile implib

exports.s : exports.js
	node exports.js

include ../sdcc.mk

