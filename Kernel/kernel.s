	.module kernel

    ; Imports from user program
    .globl  _user_init
    .globl  _user_isr

    ; Entry points into user program
    _user_init = 0x8000
    _user_isr = 0x8003

	.area   _CODE

