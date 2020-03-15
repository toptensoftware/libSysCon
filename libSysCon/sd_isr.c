#include "libSysCon.h"
#include <string.h>

SIGNAL g_sig_sd;

void sd_yield_impl()
{
    wait_signal(&g_sig_sd);
}

void sd_init_isr()
{
    init_signal(&g_sig_sd);
    sd_yield = sd_yield_impl;
}


// Check interrupt flags and fire appropriate signals
void sd_isr()
{
    if (InterruptControllerPort & IRQ_SD_CARD)
        set_signal(&g_sig_sd);
}

