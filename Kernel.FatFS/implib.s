    .module fatfs

    .globl _imp_f_init
    .globl _imp_f_mount
    .globl _imp_f_open
    .globl _imp_f_close
    .globl _imp_f_read
    .globl _imp_f_write
    .globl _imp_f_lseek
    .globl _imp_f_current_sector
    .globl _imp_f_create_sector
    .globl _imp_f_unlink
    .globl _imp_f_rename
    .globl _imp_f_opendir
    .globl _imp_f_closedir
    .globl _imp_f_readdir
    .globl _imp_f_mkdir
    .globl _imp_f_truncate
    .globl _imp_pattern_matching
    .globl _imp_malloc
    .globl _imp_free
    .globl _imp_realloc

    _imp_f_init = 0x8000
    _imp_f_mount = 0x8003
    _imp_f_open = 0x8006
    _imp_f_close = 0x8009
    _imp_f_read = 0x800c
    _imp_f_write = 0x800f
    _imp_f_lseek = 0x8012
    _imp_f_current_sector = 0x8015
    _imp_f_create_sector = 0x8018
    _imp_f_unlink = 0x801b
    _imp_f_rename = 0x801e
    _imp_f_opendir = 0x8021
    _imp_f_closedir = 0x8024
    _imp_f_readdir = 0x8027
    _imp_f_mkdir = 0x802a
    _imp_f_truncate = 0x802d
    _imp_pattern_matching = 0x8030
    _imp_malloc = 0x8033
    _imp_free = 0x8036
    _imp_realloc = 0x8039
