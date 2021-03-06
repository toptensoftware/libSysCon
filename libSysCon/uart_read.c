#include "libSysCon.h"
#include <string.h>

void (*uart_read_yield)() = yield_nop;


void uart_read_unsafe(void* ptr, uint8_t length) __naked
{
	// Write data
__asm
	push iy
	ld  iy, #0
	add	iy, sp
	ld	l, 4 (iy)
	ld  h, 5 (iy)
	ld  b, 6 (iy)
	ld	c, #UART_RX_DATA_PORT
	inir 
	pop iy
	ret
__endasm;
}

uint8_t uart_read(void* ptr, uint8_t length)
{
	char* ptr_c = (char*)ptr;
	uint8_t originalLength = length;
	while (length)
	{
		// Work out how much we can send
		uint8_t recv = UartRxStatusPort;
		if (recv > length)
			recv = length;

		// Quit if nothing to read
		if (recv == 0)
			break;

		// Read it
		uart_read_unsafe(ptr_c, recv);

		// Update length
		ptr_c += recv;
		length -= recv;
	}

	// If didn't read anything then yield
	if (length == originalLength)
	{
		uart_read_yield();
	}

	return originalLength - length;
}

uint8_t uart_read_wait(void* ptr, uint8_t length)
{
	char* p = (char*)ptr;
	char* pEnd = p + length;
	while (p < pEnd)
	{
		uint8_t r = uart_read(p, pEnd - p);
		p += r;
	}
	return length;
}
