#ifndef _LIBSYSCON_H
#define _LIBSYSCON_H

#include <stdlib.h>
#include <stdint.h>


// ------------------------- Scheduling -------------------------

// Yield back to either the emulated machine, or perhaps
// nowhere and immediately return - depending if emulated machine
// booted and running yet.
extern void (*yield)();


// ------------------------- Serial Port -------------------------

// UART Port Numbers
#define UART_TX_STATUS_PORT 0x80
#define UART_TX_DATA_PORT   0x80
#define UART_RX_STATUS_PORT 0x81
#define UART_RX_DATA_PORT   0x82

#define UART_TX_BUFFER_SIZE 255
#define UART_RX_BUFFER_SIZE 255

// Reading this port returns the number of bytes currently 
// in the transmit buffer.
__sfr __at UART_TX_STATUS_PORT UartTxStatusPort;

// Writing to this port enqueues data to the transmit buffer
// (unless it's full)
__sfr __at UART_TX_DATA_PORT UartTxDataPort;

// Reading this port returns the number of bytes currently in 
// the receive buffer.
__sfr __at UART_RX_STATUS_PORT UartRxStatusPort;

// Reading this port returns a single byte from the receive
// buffer (unless it's empty)
__sfr __at UART_RX_DATA_PORT UartRxDataPort;

// UART Write
void uart_write(void* ptr, uint8_t length);
void uart_write_sz(const char* psz);

// UART Read
uint8_t uart_read(void* ptr, uint8_t length);

#endif      // _LIBSYSCON_H