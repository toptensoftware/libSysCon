#include "libSysCon.h"
#include "stdio.h"

#define EVENT_BUFFER_SIZE	8		// Must be power of two

static MSG g_eventBuffer[EVENT_BUFFER_SIZE];
static uint8_t g_eventBufferWritePos = 0;
static uint8_t g_eventBufferReadPos = 0;
static SIGNAL g_sig_msg;

// Initialize the messaging sub-system
void msg_init_isr()
{
    init_signal(&g_sig_msg);
}

// Helper to copy a message
void msg_copy(MSG* dest, MSG* src)
{
	dest->message = src->message;
	dest->param1 = src->param1;
	dest->param2 = src->param2;
}

// Enqueue a message in the message queue
void msg_enqueue(MSG* pMessage)
{
	uint8_t nextWritePos;

	// Write it to the queue
	nextWritePos = (g_eventBufferWritePos + 1) & (EVENT_BUFFER_SIZE-1);
	if (nextWritePos != g_eventBufferReadPos)
	{
		msg_copy(&g_eventBuffer[g_eventBufferWritePos], pMessage);
		g_eventBufferWritePos = nextWritePos;

        set_signal(&g_sig_msg);
	}
}

// Post a message to the message queue
void msg_post(uint8_t message, uint8_t param1, uint16_t param2)
{
	MSG msg;
	msg.message = message;
	msg.param1 = param1;
	msg.param2 = param2;
	msg_enqueue(&msg);
}

// Peek and optionally remove the next message from the message queue
bool msg_peek(MSG* pMsg, bool remove)
{
    // Anything in the queue?
    if (g_eventBufferReadPos == g_eventBufferWritePos)
        return false;

    // Yep
    msg_copy(pMsg, &g_eventBuffer[g_eventBufferReadPos]);

    if (remove)
    {
        g_eventBufferReadPos = (g_eventBufferReadPos + 1) & (EVENT_BUFFER_SIZE-1);
    }

    return true;
}

// Get the next message from the message queue, yielding until something available
void msg_get(MSG* pMsg)
{
	while (true)
	{
		if (msg_peek(pMsg, true))
			return;

        msg_yield();
	}
}

// Yield the current fiber until a new message is enqueued
void msg_yield()
{
    wait_signal(&g_sig_msg);
}


// Messaging ISR
void msg_isr()
{
    // Quit if keyboard IRQ not raised?
    if (InterruptControllerPort & IRQ_KEYBOARD)
    {
        // Currenlly there isn't a queue of keyboard events in the fpga, but assume
        // there might be one day.
        while (true)
        {
            // Read the scan code, quit if no more
            uint8_t lo = KeyboardPortLo;
            if (lo == 0)
                break;

            // Read the modifiers etc... (and clear the irq)
            uint8_t hi = KeyboardPortHi;

            // Post message
            msg_post((hi & 0x80) ? MESSAGE_KEYUP : MESSAGE_KEYDOWN, lo, hi);

            // Also generate character messages?
            if ((hi & 0x80) == 0)
            {
                char ch = key_to_char(lo, hi);
                if (ch != 0)
                {
                    msg_post(MESSAGE_CHAR, ch, 0);
                }
            }
        }
    }

}
