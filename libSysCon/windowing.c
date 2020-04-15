#include "libSysCon.h"

static WINDOW* g_pActiveWindow = NULL;

size_t window_send_message(WINDOW* pWindow, uint8_t message, uint8_t param1, uint16_t param2)
{
	MSG msg;
	msg.message = message;
	msg.param1 = param1;
	msg.param2 = param2;
	return pWindow->wndProc(pWindow, &msg);
}

void window_get_client_rect(WINDOW* pWindow, RECT* prc)
{
	prc->left = pWindow->rcFrame.left + 1;
	prc->top = pWindow->rcFrame.top + 1;
	prc->width = pWindow->rcFrame.width - 2;
	prc->height = pWindow->rcFrame.height - 2;
}


size_t window_def_proc(WINDOW* pWindow, MSG* pMsg)
{
	switch (pMsg->message)
	{
		case MESSAGE_DRAWFRAME:
			// Draw background
			video_draw_box_indirect(&pWindow->rcFrame, window_get_attr_normal(pWindow));

			// Draw title
			if (pWindow->title[0])
				video_draw_text(pWindow->rcFrame.left + 1, pWindow->rcFrame.top, pWindow->rcFrame.width - 2, pWindow->title, DT_CENTER | DT_NOFILL);
			break;
	}
	return 0;
}

WINDOW* window_get_active()
{
	return g_pActiveWindow;
}

// Create a window and add it to the stack
void window_create(WINDOW* pWindow)
{
	// Setup the new window, capture underlying screen
	pWindow->parent = g_pActiveWindow;
	pWindow->screenSave = NULL;
	pWindow->modal = false;
	pWindow->running = false;
	pWindow->retv = 0;

	// Make it active
	g_pActiveWindow = pWindow;

	video_begin_offscreen();

	// Tell the parent window to redraw itself inactive
	if (pWindow->parent)
	{
		window_send_message(pWindow->parent, MESSAGE_DRAWFRAME, 0, 0);
		window_send_message(pWindow->parent, MESSAGE_DRAWCONTENT, 0, 0);
	}

	// Save the underlying characters
	pWindow->screenSave = video_save(pWindow->rcFrame.left, pWindow->rcFrame.top, pWindow->rcFrame.width, pWindow->rcFrame.height);

	// Draw this window
	window_send_message(pWindow, MESSAGE_DRAWFRAME, 0, 0);
	window_send_message(pWindow, MESSAGE_DRAWCONTENT, 0, 0);

	pWindow->needsDraw = false;

	video_end_offscreen();
}

// Destroy the top window
void window_destroy()
{
	// Restore the underlying characters
	WINDOW* pWindow = g_pActiveWindow;

	video_begin_offscreen();

	// Remove from screen
	if (pWindow->screenSave)
	{
		video_restore(pWindow->screenSave);
		pWindow->screenSave = NULL;
	}

	// Deactivate
	g_pActiveWindow = pWindow->parent;

	// Redraw parent window
	if (g_pActiveWindow)
	{
		window_send_message(g_pActiveWindow, MESSAGE_DRAWFRAME, 0, 0);
		window_send_message(g_pActiveWindow, MESSAGE_DRAWCONTENT, 0, 0);
		g_pActiveWindow->needsDraw = false;
	}

	video_end_offscreen();

	// Done!
}

void window_run_modeless(WINDOW* pWindow)
{
	// Create the window
	window_create(pWindow);

	// Mark as running
	pWindow->running = true;
}

PFNWINDOWHOOK window_msg_hook = NULL;

void window_set_hook(PFNWINDOWHOOK pfn)
{
	window_msg_hook = pfn;
}

bool window_update_modeless(WINDOW* pWindow)
{
	// Dispatch all pending messages
	MSG msg;
	while (pWindow->running && msg_peek(&msg, true))
	{
		if (window_msg_hook != NULL)
		{
			bool bHandled = false;
			size_t retv = window_msg_hook(pWindow, &msg, &bHandled);
			if (bHandled)
				continue;
		}

		pWindow->wndProc(pWindow, &msg);
	}

	// Paint?
	if (pWindow->running && pWindow->needsDraw)
	{
		pWindow->needsDraw = false;
		window_send_message(pWindow, MESSAGE_DRAWCONTENT, 0, 0);
	}

	// Continue
	return pWindow->running;
}

int window_end_modeless(WINDOW* pWindow)
{
	// Kill it
	window_destroy();
	return pWindow->retv;	
}

// Run window modally (don't call create window first)
int window_run_modal(WINDOW* pWindow)
{
	window_run_modeless(pWindow);
	pWindow->modal = true;

	while (window_update_modeless(pWindow))
	{
		msg_yield();
	}

	return window_end_modeless(pWindow);
}

// End a modal window
void window_end_modal(int retv)
{
	g_pActiveWindow->running = false;
	g_pActiveWindow->retv = retv;

	// make sure message loop spins to we return to caller
    msg_post(MESSAGE_NOP, 0, 0);
}


uint8_t window_get_attr_normal(WINDOW* pWindow)
{
	if (g_pActiveWindow == pWindow)
		return pWindow->attrNormal;
	else
		return MAKECOLOR(COLOR_WHITE, COLOR_DARKGRAY);
}

uint8_t window_get_attr_selected(WINDOW* pWindow)
{
	if (g_pActiveWindow == pWindow)
		return pWindow->attrSelected;
	else
		return MAKECOLOR(COLOR_DARKGRAY, COLOR_WHITE);
}

void window_invalidate(WINDOW* pWindow)
{
	pWindow->needsDraw = true;
}