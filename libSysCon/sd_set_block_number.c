#include "libSysCon.h"
#include <string.h>

void sd_set_block_number(uint32_t blockNumber) __naked
{
__asm
		ld	hl, #2
		add hl,sp

        ; B = length (4 bytes)
        ; C = port number
		ld	bc,#0x0400 + SD_PORT_SET_BLOCK_NUMBER
		otir
		ret
__endasm;
}

