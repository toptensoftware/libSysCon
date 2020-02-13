#include "libSysCon.h"
#include <string.h>

void sd_read_command(uint32_t blockNumber)
{
	// Set block number
	sd_set_block_number(blockNumber);

	// command command
	SdCommandPort = SD_COMMAND_READ;

	// Wait for it
	while (SdStatusPort & SD_STATUS_BUSY)
		sd_yield();
}

void sd_read_data(void* ptr) __naked
{
__asm
	ld	hl, #2
	add	hl, sp
	ld	a, (hl)
	inc	hl
	ld	h, (hl)
	ld	l, a

	; B = length (256 bytes)
	; C = port number
	ld	bc,#0x0000 + SD_PORT_DATA
	inir
	inir
	ret
__endasm;
}

void sd_read(uint32_t block_number, void* ptr)
{
	sd_read_command(block_number);
	sd_read_data(ptr);
}
