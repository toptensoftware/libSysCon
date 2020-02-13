#ifndef _LIBSYSCON_H
#define _LIBSYSCON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



// ------------------------- Linked Lists -------------------------

typedef struct _LISTITEM
{
	struct _LISTITEM* pNext;
} LISTITEM;


void* ll_push(void** pHead, void* pItem);
void* ll_pop(void** pHead);
void* ll_shift(void** pHead);
void* ll_unshift(void** pHead, void* pItem);
void* ll_remove(void** pHead, void* pItem);
bool ll_contains(void* pHead, void* pItem);



// ------------------------- NMI handling -------------------------

// Call this to yield back from the NMI handler
void yield_from_nmi();



// ------------------------- Fibers -------------------------

typedef struct _FIBER
{
    struct _FIBER* pNext;
    void* sp;
    bool sleeping;
} FIBER;

typedef struct _SIGNAL
{
    FIBER* pWaitingFibers;
    bool set;
} SIGNAL;

typedef struct _MUTEX
{
    FIBER* pOwningFiber;
    SIGNAL signal;
} MUTEX;

// Run all active fibers
void run_fibers();

// Create a new fiber with given proc and stack size
FIBER* create_fiber(void (*fiberProc)(), int stack_size);

// Signals
void init_signal(SIGNAL* pSignal);
void set_signal(SIGNAL* pSignal);
void wait_signal(SIGNAL* pSignal);

// Mutexes
void init_mutex(MUTEX* pMutex);
void enter_mutex(MUTEX* pMutex);
void leave_mutex(MUTEX* pMutex);



// ------------------------- Address Page Mapping -------------------------

// Address page mapping ports
#define APM_PAGEBANK_PORT     0xA1
#define APM_ENABLE_PORT       0xA2

// Sets the upper bits that addresses 0xFC00 -> 0xFFFF map to
// Actual ram address will be (ApmPageBank << 10) | address (& 0x3ff)
__sfr __at APM_PAGEBANK_PORT ApmPageBank;

// Enables/disables other entries in the memory map (see SYSCON_ENABLE_* flags)
__sfr __at APM_ENABLE_PORT ApmEnable;

// Bit flags for ApmSysConEnable
#define APM_ENABLE_VIDEO_RAM  0x01           // 0xFC00 -> 0xFFFF
#define APM_ENABLE_BOOTROM    0x02           // Enable boot rom at 0x0000 -> 0x7FFF
                                             // (from 0x6000 -> 0x7FFF is writable)
#define APM_ENABLE_PAGEBANK   0x04           // Enable page banking from 0xFC00 -> 0xFFFF

__at(0xFC00) char banked_page[0x400];



// ------------------------- Interrupt Controller -------------------------

#define INTERRUPT_CONTROLLER_PORT    0x1c

__sfr __at INTERRUPT_CONTROLLER_PORT InterruptControllerPort;

// Reading from the interrupt controller port returns a bit mask
// of the raised interrupts.

// Writing to the interrupt controller port indicates a request
// to exit hijack mode.  The actual mode switch will happen on the 
// next RET or JP (HL).



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


extern void (*uart_read_yield)();
extern void (*uart_write_yield)();

// UART ISR
void uart_read_init_isr();
void uart_read_isr();
void uart_write_init_isr();
void uart_write_isr();



// ------------------------- SD Card -------------------------

#define SD_PORT_SET_BLOCK_NUMBER    0x90        // Write-only, 32-bits, LSB first
#define SD_PORT_STATUS              0x91        // Read for status
#define SD_PORT_COMMAND             0x91        // Write to invoke command
#define SD_PORT_DATA                0x92        // Read-Write

#define SD_STATUS_BUSY			    0x01 	    // Disk controller is busy
#define SD_STATUS_READING           0x02        // Dick controller is busy reading
#define SD_STATUS_WRITING           0x04        // Disk controller is busy writing
#define SD_STATUS_ERROR			    0x08        // Disk controller has an DISK_STATUS_BIT_ERROR
#define SD_STATUS_INIT      	    0x40        // Disk controller has initialized
#define SD_STATUS_SDHC			    0x80 	    // Disk controller is busy

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

void sd_set_block_number(uint32_t blockNumber);

void sd_read_command(uint32_t blockNumber);
void sd_read_data(void* ptr);
void sd_read(uint32_t block_number, void* ptr);

void sd_write_command(uint32_t blockNumber);
void sd_write_data(void* ptr);
void sd_write(uint32_t block_number, void* ptr);

extern void (*sd_yield)();



// ------------------------- No-op Yield -------------------------

void yield_nop();


#endif      // _LIBSYSCON_H