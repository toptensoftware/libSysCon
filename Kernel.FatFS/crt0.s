;--------------------------------------------------------------------------
;  crt0.s - Generic crt0.s for a Z80
;
;  Copyright (C) 2000, Michael Hope
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.module crt0
	.globl    s__DATA
	.globl    l__DATA
	.globl    l__INITIALIZER
    .globl    s__INITIALIZER
    .globl    s__INITIALIZED
	.globl		___sdcc_heap_init
	.globl		___sdcc_heap
	.globl 		___sdcc_heap_end
	.globl    _f_init
	.area	_HEADER (ABS)

	;; From 0x8000 populated from exports
	.org	0x8000

___sdcc_heap = 0x5800
___sdcc_heap_end = 0x7fff

	.org 	0x8080
_f_init:
	;; Initialise global variables
	call	gsinit
	call    ___sdcc_heap_init
	ret

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

