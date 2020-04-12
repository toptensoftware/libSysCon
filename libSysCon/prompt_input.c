#include "libSysCon.h"
#include <string.h>
#include <stdio.h>

typedef struct tagPROMPTINPUT
{
	WINDOW window;
	int cursorPos;
	char* workBuf;
	int cbBuf;
} PROMPTINPUT;

extern char g_szTemp[];

size_t prompt_input_wndproc(WINDOW* pWindow, MSG* pMsg)
{
	PROMPTINPUT* p = (PROMPTINPUT*)pWindow;

	switch (pMsg->message)
	{
		case MESSAGE_DRAWCONTENT:
		{
			uint8_t save = ApmEnable;
			ApmEnable |= APM_ENABLE_VIDEOBANK;
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
			ApmEnable = save;
			break;
		}

		case MESSAGE_CHAR:
			if (p->cursorPos < p->cbBuf && pMsg->param1 >= 32)
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

const char* prompt_input(const char* pszTitle, const char* buf)
{
	// Setup window
	PROMPTINPUT prompt;
	memset(&prompt, 0, sizeof(prompt));
	prompt.window.rcFrame.left = 0;
	prompt.window.rcFrame.top = 8;
	prompt.window.rcFrame.width = SCREEN_WIDTH;
	prompt.window.rcFrame.height = 3;
	prompt.window.attrNormal = MAKECOLOR(COLOR_WHITE, COLOR_BLUE);
	prompt.window.attrSelected = MAKECOLOR(COLOR_BLACK, COLOR_YELLOW);
	prompt.window.title = pszTitle;
	prompt.window.wndProc = prompt_input_wndproc;
	prompt.cbBuf = 64;
	prompt.workBuf = malloc(prompt.cbBuf);

	// Setup prompt
	if (buf)
	{
		prompt.cursorPos = strlen(buf);
		if (prompt.cursorPos)
			memcpy(prompt.workBuf, buf, prompt.cursorPos);
	}
	else
	{
		prompt.cursorPos = 0;
	}

	// Run it...
	if (!window_run_modal(&prompt.window))
	{
		free(prompt.workBuf);
		return NULL;
	}

	// Return entered text to caller
	prompt.workBuf[prompt.cursorPos] = '\0';
	const char* pszRet = stralloc(prompt.workBuf);
	free(prompt.workBuf);
	return pszRet;
}
	