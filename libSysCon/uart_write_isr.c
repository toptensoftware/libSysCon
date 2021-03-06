#include "libSysCon.h"
#include <string.h>

SIGNAL g_sig_uart_tx;

void uart_yield_tx_impl()
{
    wait_signal(&g_sig_uart_tx);
}

void uart_write_init_isr()
{
    init_signal(&g_sig_uart_tx);
    uart_write_yield = uart_yield_tx_impl;
}

// Check interrupt flags and fire appropriate signals
void uart_write_isr()
{
    if (InterruptControllerPort & IRQ_UART_TX)
        set_signal(&g_sig_uart_tx);
}

