#include "libSysCon.h"
#include <string.h>

const char* okButtons[] = { "OK", NULL };
const char* okCancelButtons[] = { "OK", "Cancel", NULL };
const char** buttonTexts[] = { okButtons, okCancelButtons };
/*
const char* yesNoButtons[] = { "Yes", "No", NULL };
const char* stopButtons[] = { "Stop", NULL };
const char* cancelButtons[] = { "Cancel", NULL };
const char* stopCancelButtons[] = { "Cancel", NULL };
*/

int button_width(const char** ppsz)
{
	int length = 0;
	while (*ppsz)
	{
		length += strlen(*ppsz) + 2;
		ppsz++;
	}
	return length;
}

int button_count(const char** ppsz)
{
	int count = 0;
	while (*ppsz)
	{
		count++;
		ppsz++;
	}
	return count;
}

size_t message_box_wndproc(WINDOW* pWindow, MSG* pMsg)
{
	MESSAGEBOX* pmb = (MESSAGEBOX*)pWindow;
	switch (pMsg->message)
	{
		case MESSAGE_DRAWCONTENT:
		{
			RECT rcClient;
			POINT pos;
			int buttonXPos;
			int button = 0;

			// Draw the message
			window_get_client_rect(pWindow, &rcClient);
			pos.x = rcClient.left + 1;
			pos.y = rcClient.top + 1;
			video_draw_multiline_text(pmb->pszMessage, &pos);

			// Draw the buttons
			const char** ppszButtons = pmb->ppszButtons;
			buttonXPos = rcClient.left + (rcClient.width - button_width(ppszButtons))/2;
			while (*ppszButtons)
			{
				video_set_color(buttonXPos, rcClient.top + rcClient.height - 1, strlen(*ppszButtons) + 2, 1, 
						button == pmb->selectedButton ? pWindow->attrSelected : pWindow->attrNormal);
				video_text_out(buttonXPos + 1, rcClient.top + rcClient.height - 1, *ppszButtons);
				buttonXPos += 2 + strlen(*ppszButtons);
				ppszButtons++;	
				button++;
			}
			break;
		}

		case MESSAGE_KEYDOWN:
		{
			switch (pMsg->param1)
			{
				case KEY_LEFT:
					if (pmb->selectedButton > 0)
					{
						pmb->selectedButton--;
						window_invalidate(pWindow);
					}
					break;

				case KEY_RIGHT:
				{
					if (pmb->selectedButton + 1 < button_count(pmb->ppszButtons))
					{
						pmb->selectedButton++;
						window_invalidate(pWindow);
					}
					break;
				}

				case KEY_ESCAPE:
					window_end_modal(0);
					break;

				case KEY_ENTER:
					if (pmb->selectedButton == button_count(pmb->ppszButtons) - 1)
						window_end_modal(0);	// Cancel
					else
						window_end_modal(pmb->selectedButton+1);	// 1, 2, 3...
					break;
			}
		}

	}
	return window_def_proc(pWindow, pMsg);
}

void message_box_modeless(MESSAGEBOX* pMessageBox, const char* pszTitle, const char* pszMessage, const char** ppszButtons, uint8_t flags)
{
	POINT textSize;
	int temp;
	bool error = (flags & MBF_ERROR)!=0;
	bool progress = (flags & MBF_INPROGRESS)!=0;

	// Measure text
	video_measure_multiline_text(pszMessage, &textSize);

	// Work out width of buttons
	temp = button_width(ppszButtons);
	if (temp > textSize.x)
		textSize.x = temp;

	// Work out width of title
	temp = strlen(pszTitle);
	if (temp > textSize.x)
		textSize.x = temp;

	// Setup window
	memset(pMessageBox, 0, sizeof(*pMessageBox));
	pMessageBox->window.rcFrame.left = 0;
	pMessageBox->window.rcFrame.top = SCREEN_HEIGHT - 3;
	pMessageBox->window.rcFrame.width = textSize.x + 4;
	pMessageBox->window.rcFrame.height = textSize.y + 5;
	pMessageBox->window.rcFrame.left = (SCREEN_WIDTH - pMessageBox->window.rcFrame.width) / 2;
	pMessageBox->window.rcFrame.top = (SCREEN_HEIGHT - pMessageBox->window.rcFrame.height) / 2;
	pMessageBox->window.attrNormal = MAKECOLOR(COLOR_WHITE, error ? COLOR_DARKRED : (progress ? COLOR_DARKGREEN : COLOR_BLUE));
	pMessageBox->window.attrSelected = MAKECOLOR(COLOR_BLACK, COLOR_YELLOW);
	pMessageBox->window.title = pszTitle;
	pMessageBox->window.wndProc = message_box_wndproc;
	pMessageBox->pszMessage = pszMessage;
	pMessageBox->ppszButtons = ppszButtons;
	pMessageBox->flags = flags;
	pMessageBox->selectedButton = 0;
}

int message_box(const char* pszTitle, const char* pszMessage, const char** ppszButtons, uint8_t flags)
{
	if (((uint16_t)ppszButtons) < countof(buttonTexts))
	{
		ppszButtons = buttonTexts[((uint16_t)ppszButtons) - 1];
	}

	// Setup
	MESSAGEBOX mb;
	message_box_modeless(&mb, pszTitle, pszMessage, ppszButtons, flags);

	// Run it...
	return window_run_modal(&mb.window);
}
