#include "libSysCon.h"
#include <string.h>

typedef struct tagPROMPTINPUT
{
	WINDOW window;
	int cursorPos;
	char workBuf[64];
	int cbBuf;
} PROMPTINPUT;

size_t prompt_input_wndproc(WINDOW* pWindow, MSG* pMsg)
{
	PROMPTINPUT* p = (PROMPTINPUT*)pWindow;

	switch (pMsg->message)
	{
		case MESSAGE_DRAWCONTENT:
		{
			RECT rcClient;
			char* pszScreenBuf;
			window_get_client_rect(pWindow, &rcClient);
			pszScreenBuf = video_char_ram + rcClient.left + rcClient.top * SCREEN_WIDTH;
			memcpy(pszScreenBuf, p->workBuf, p->cursorPos);
			if (p->cursorPos + 1 < rcClient.width)
			{
				memset(pszScreenBuf + p->cursorPos, ' ', rcClient.width - p->cursorPos);
			}
			pszScreenBuf[p->cursorPos] = CH_CURSOR;
			break;
		}

		case MESSAGE_CHAR:
			if (p->cursorPos < p->cbBuf)
			{
				p->workBuf[p->cursorPos] = (char)pMsg->param1;
				p->cursorPos++;
				window_invalidate(pWindow);
			}
			break;

		case MESSAGE_KEYDOWN:
		{
			switch (pMsg->param1)
			{
				case KEY_ESCAPE:
					if (p->window.modal)
						window_end_modal(0);
					break;

				case KEY_ENTER:
					if (p->window.modal)
						window_end_modal(1);
					break;

				case KEY_BACKSPACE:
					if (p->cursorPos > 0)
					{
						p->cursorPos--;
						p->workBuf[p->cursorPos] = ' ';
						window_invalidate(pWindow);
						break;
					}
			}
			break;
		}
	}
	return window_def_proc(pWindow, pMsg);
}

bool prompt_input(const char* pszTitle, char* buf, int cbBuf)
{
	// Setup window
	PROMPTINPUT prompt;
	memset(&prompt, 0, sizeof(prompt));
	prompt.window.rcFrame.left = 0;
	prompt.window.rcFrame.top = SCREEN_HEIGHT - 3;
	prompt.window.rcFrame.width = SCREEN_WIDTH;
	prompt.window.rcFrame.height = 3;
	prompt.window.attrNormal = MAKECOLOR(COLOR_WHITE, COLOR_BLUE);
	prompt.window.attrSelected = MAKECOLOR(COLOR_BLACK, COLOR_YELLOW);
	prompt.window.title = pszTitle;
	prompt.window.wndProc = prompt_input_wndproc;

	// Setup prompt
	prompt.cursorPos = strlen(buf);
	if (prompt.cursorPos)
		memcpy(prompt.workBuf, buf, prompt.cursorPos);
	prompt.cbBuf = cbBuf;

	// Run it...
	if (!window_run_modal(&prompt.window))
		return false;

	// Return entered text to caller
	if (prompt.cursorPos)
		memcpy(buf, prompt.workBuf, prompt.cursorPos);
	buf[prompt.cursorPos] = '\0';
	return true;
}
	