    .module exports

    .globl _f_init
    .globl _f_mount
    .globl _f_open
    .globl _f_close
    .globl _f_read
    .globl _f_write
    .globl _f_lseek
    .globl _f_current_sector
    .globl _f_create_sector
    .globl _f_unlink
    .globl _f_rename
    .globl _f_opendir
    .globl _f_closedir
    .globl _f_readdir
    .globl _f_mkdir
    .globl _f_truncate
    .globl _pattern_matching
    .globl _malloc
    .globl _free
    .globl _realloc

    .area	_EXPORTS (ABS)
    .org 0x8000

    jp _f_init	; 0x8000
    jp _f_mount	; 0x8003
    jp _f_open	; 0x8006
    jp _f_close	; 0x8009
    jp _f_read	; 0x800c
    jp _f_write	; 0x800f
    jp _f_lseek	; 0x8012
    jp _f_current_sector	; 0x8015
    jp _f_create_sector	; 0x8018
    jp _f_unlink	; 0x801b
    jp _f_rename	; 0x801e
    jp _f_opendir	; 0x8021
    jp _f_closedir	; 0x8024
    jp _f_readdir	; 0x8027
    jp _f_mkdir	; 0x802a
    jp _f_truncate	; 0x802d
    jp _pattern_matching	; 0x8030
    jp _malloc	; 0x8033
    jp _free	; 0x8036
    jp _realloc	; 0x8039
