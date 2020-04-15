#include <stdio.h>
#include <string.h>

#include "kernel.h"

/*

Address Map

0x0000 - 0x0100     Entry point and interrupt vector table
0x0100 - 0x0300     Exported function vector table
0x0300 - 0x0400     g_szTemp
0x0400 - 0x5200     CODE
0x5200 - 0x5600     DATA
0x5600 - 0x5800     System Stack (ISR handlers)
0x5800 - 0x8000     HEAP

*/

// Assign to ApmPageBank to control memory mapped into high 32k

                                   // --bank-- 14....8 7......0   LPDDR
#define BANK_TRS80_LO 0x00         // 00000000|0000000 00000000  0x00000
#define BANK_TRS80_HI 0x01         // 00000001|0000000 00000000  0x08000
#define BANK_KERNEL   0x02         // 00000010|0000000 00000000  0x10000
#define BANK_FATFS    0x03         // 00000011|0000000 00000000  0x18000
#define BANK_USER     0x04         // 00000100|0000000 00000000  0x20000


#define ENTER_BANK(bankNumber) \
    uint8_t saveBank = ApmPageBank32k; \
    ApmPageBank32k = bankNumber;

#define LEAVE_BANK() \
    ApmPageBank32k = saveBank;
