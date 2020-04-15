#include "common.h"

FATFS g_fs;

// Initialize fatfs.sys. See ff.c
FRESULT f_init();

// Entry points to user program
bool user_init();       // 0x8000
void user_isr();        // 0x8003

// Main Entry Point
void main(void) 
{
    // Enable 32k page banking and map in the user program
    ApmPageBank32k = BANK_USER;

    // initialize fatfs.sys and heap
    f_init();

    // Hello!
    uart_write_sz("Kernel loaded!\n");

    // Mount SD Card
    uart_write_sz("Mounting SD card...");
    FRESULT r = f_mount(&g_fs, "0", 1);
    if (r != 0)
    {
        sprintf(g_szTemp, " FAILED (%i)\n", r);
        uart_write_sz(g_szTemp);
        return;
    }
    uart_write_sz(" OK\n");

    // Initialie user program
    if (!user_init())
    {
        uart_write_sz("user_init failed\n");
        return;
    }

    // Initialize ISR handlers
    uart_read_init_isr();
    uart_write_init_isr();
    sd_init_isr();
    msg_init_isr();

    // Main processing loop
    while (true)
    {
        // Run all active fibers
        run_fibers();

        // Yield
        yield_from_nmi();

        // Process all interrupts
        uart_read_isr();
        uart_write_isr();
        sd_isr();
        msg_isr();
        user_isr();
    }
}
