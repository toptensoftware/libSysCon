	.module crt0
	.globl	_main
	.globl    s__DATA
	.globl    l__DATA
	.globl    l__INITIALIZER
    .globl    s__INITIALIZER
    .globl    s__INITIALIZED
    .globl  _nmi_handler
	.globl  _g_szTemp

	
	
	;;
	;; Entry Point
	;;

	.area	_HEADER (ABS)
	.org 	0
	jp	init



	;;
	;; Interrupt vector table
	;; 

	.org	0x08
	reti
	.org	0x10
	reti
	.org	0x18
	reti
	.org	0x20
	reti
	.org	0x28
	reti
	.org	0x30
	reti
	.org	0x38
	reti
	.org	0x66
	jp		_nmi_handler	

	
	
	;;
	;; Export function vector table goes here (see exports.js/.s)
	;;
	
	.org	0x100


	
	;;
	;; Fixed temporary work buffer
	;;

	.org    0x300
_g_szTemp:
	.ds		0x100


	;;
	;; Initialization code
	;;

	.area	_CODE
init:

	;; Setup stack pointer
	ld	sp,#0x5800

	;; Initialise global variables
	call	gsinit

	;; Go...
	call	_main

	;; Should never get here
2$:
	jr 		2$


	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL

	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP

	.area   _CODE

	.area   _GSINIT
gsinit::
	ld  hl, #s__DATA
	xor a
	ld  (hl),a
	ld  bc, #(l__DATA - 1)
	ld  a,b
	or  a,c
	jr  z, gsinit_1
	ld  de, #(s__DATA + 1)
	ldir

gsinit_1::
	ld	bc, #l__INITIALIZER
	ld  a,b
	or  a,c
	jr  z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir


gsinit_next:

	.area   _GSFINAL
	ret

