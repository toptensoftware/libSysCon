#include "libSysCon.h"
#include <string.h>

SIGNAL g_sig_uart_rx;

void uart_yield_rx_impl()
{
    wait_signal(&g_sig_uart_rx);
}

void uart_read_init_isr()
{
    init_signal(&g_sig_uart_rx);
    uart_read_yield = uart_yield_rx_impl;
}


// Check interrupt flags and fire appropriate signals
void uart_read_isr()
{
    if (InterruptControllerPort & 0x01)
        fire_signal(&g_sig_uart_rx);
}

