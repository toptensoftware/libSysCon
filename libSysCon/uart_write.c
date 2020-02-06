#include "libSysCon.h"
#include <string.h>

void uart_write_unsafe(void* ptr, uint8_t length) __naked
{
	// Write data
__asm
	ld  iy,#0
	add	iy,sp
	ld	l, 2 (iy)
	ld  h, 3 (iy)
	ld  b, 4 (iy)
	ld	c,#UART_TX_DATA_PORT
	otir
	ret
__endasm;
}

void uart_write(void* ptr, uint8_t length)
{
	char* ptr_c = (char*)ptr;
	while (length)
	{
		// Work out how much we can send
		uint8_t send = UART_TX_BUFFER_SIZE - UartTxStatusPort;
		if (send > length)
			send = length;

		// Yield if can't send anything
		if (send == 0)
		{
			yield();
			continue;
		}

		// Write it
		uart_write_unsafe(ptr_c, send);

		// Update length
		ptr_c += send;
		length -= send;
	}
}

void uart_write_sz(const char* psz)
{
	uart_write(psz, strlen(psz));
}
