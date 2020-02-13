#include "libSysCon.h"
#include <string.h>

void sd_write_command(uint32_t blockNumber)
{
	// Set block number
	sd_set_block_number(blockNumber);

	// command command
	SdCommandPort = SD_COMMAND_WRITE;

	// Wait for it
	while (SdStatusPort & SD_STATUS_BUSY)
		sd_yield();
}

void sd_write_data(void* ptr) __naked
{
__asm
	ld	hl, #2
	add	hl, sp
	ld	a, (hl)
	inc	hl
	ld	h, (hl)
	ld	l, a

	; Reset buffer pointer
	xor a							; SD_COMMAND_NOP
	out	(SD_PORT_COMMAND),A

	; B = length (256 bytes)
	; C = port number
	ld	bc,#0x0000 + SD_PORT_DATA
	otir
	otir
	ret
__endasm;
}

void sd_write(uint32_t block_number, void* ptr)
{
	sd_write_data(ptr);
	sd_write_command(block_number);
}
