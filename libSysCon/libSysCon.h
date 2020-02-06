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


// ------------------------- SD Card -------------------------

#define SD_PORT_SET_BLOCK_NUMBER    0x90        // Write-only, 32-bits, LSB first
#define SD_PORT_STATUS              0x91        // Read for status
#define SD_PORT_COMMAND             0x91        // Write to invoke command
#define SD_PORT_DATA                0x92        // Read-Write

#define SD_STATUS_BUSY			    0x80 	    // Disk controller is busy
#define SD_STATUS_INITIALIZED	    0x40        // Disk controller has initialized
#define SD_STATUS_WRITING           0x04        // Disk controller is busy writing
#define SD_STATUS_READING           0x02        // Dick controller is busy reading
#define SD_STATUS_ERROR			    0x01        // Disk controller has an DISK_STATUS_BIT_ERROR

#define SD_COMMAND_NOP              0           // Null op (resets the buffer pointer to 0)
#define SD_COMMAND_READ			    1           // Start a read operation
#define SD_COMMAND_WRITE			2           // Start a write operation

// Write to this port 4x times (32-bits) to set the block number (LSB first)
__sfr __at SD_PORT_SET_BLOCK_NUMBER SdSetBlockNumberPort;

// Read this port to get the SD Card status (SD_STATUS_*)
__sfr __at SD_PORT_STATUS SdStatusPort;

// Write to this port to invoke commands (SD_COMMAND_*)
__sfr __at SD_PORT_COMMAND SdCommandPort;

// Read/write this port to drain/fill the SD card buffer
__sfr __at SD_PORT_DATA SdDataPort;


#endif      // _LIBSYSCON_H