#include "libSysCon.h"
#include <string.h>

static void listbox_draw_content(LISTBOX* pListBox, uint8_t from, uint8_t count)
{
	RECT rcClient;
	int item = pListBox->scrollPos + from;
	int itemCount = pListBox->getItemCount(pListBox);
	uint8_t row, endRow;

	window_get_client_rect(&pListBox->window, &rcClient);
 	row = rcClient.top + from;
	endRow = row + count;

	while (row < endRow)
	{
		// Draw text
		const char* psz = item < itemCount ? pListBox->getItemText(pListBox, item) : "";

		if (psz[0]=='\1')
			memset(video_char_ram + row * SCREEN_WIDTH + rcClient.left, BOX_H, rcClient.width);
		else
			video_draw_text(rcClient.left, row, rcClient.width, psz, DT_LEFT);

		row++;
		item++;
	}
}

static void listbox_draw_highlight(LISTBOX* pListBox)
{
	int highlightedRow = pListBox->selectedItem - pListBox->scrollPos;
	RECT rcClient;
	window_get_client_rect(&pListBox->window, &rcClient);

	if (highlightedRow >= 0 && highlightedRow < rcClient.height)
	{
		video_set_color(rcClient.left, highlightedRow + rcClient.top, rcClient.width, 1, window_get_attr_selected(&pListBox->window));
	}
}

void listbox_draw(LISTBOX* pListBox)
{
	RECT rcClient;
	window_get_client_rect(&pListBox->window, &rcClient);
	listbox_draw_highlight(pListBox);
	listbox_draw_content(pListBox, 0, rcClient.height);
}

void listbox_drawitem(LISTBOX* pListBox, int item)
{
	int row = item - pListBox->scrollPos;
	RECT rcClient;
	window_get_client_rect(&pListBox->window, &rcClient);
	if (row >=0 && row < rcClient.height)
	{
		listbox_draw_content(pListBox, row, 1);
	}
}

// Moves the selection and scroll position
bool listbox_update(LISTBOX* pListBox, int newSelectedItem, int newScrollPos)
{
	// Remember which row was highlighted
	int oldHighlightedRow, newHighlightedRow;
	int oldSelectedItem = pListBox->selectedItem;
	int oldScrollPos = pListBox->scrollPos;
	RECT rcClient;
	window_get_client_rect(&pListBox->window, &rcClient);

	// Redundant?
	if (newSelectedItem == oldSelectedItem && newScrollPos == oldScrollPos)
		return false;

	// Store new state
	pListBox->selectedItem = newSelectedItem;
	pListBox->scrollPos = newScrollPos;

	// Scroll
	if (newScrollPos != oldScrollPos)
	{
		uint8_t redrawFrom, redrawCount;

		// Scroll screen
		video_scroll(rcClient.left, rcClient.top, rcClient.width, rcClient.height, 
						oldScrollPos - newScrollPos, false, 
						&redrawFrom, &redrawCount
						);

		// Redraw context
		listbox_draw_content(pListBox, redrawFrom, redrawCount);
	}

	// Update highlighted row
	oldHighlightedRow = oldSelectedItem - oldScrollPos;
	newHighlightedRow = newSelectedItem - newScrollPos;
	if (oldHighlightedRow != newHighlightedRow)
	{
		// Unhighlight old row
		if (oldHighlightedRow >= 0 && oldHighlightedRow < rcClient.height)
			video_set_color(rcClient.left, rcClient.top + oldHighlightedRow, rcClient.width, 1, window_get_attr_normal(&pListBox->window));

		// Highlight new row
		listbox_draw_highlight(pListBox);
	}

	return true;
}

int listbox_ensure_visible(LISTBOX* pListBox, int item)
{
	int scrollPos = pListBox->scrollPos;
	RECT rcClient;
	window_get_client_rect(&pListBox->window, &rcClient);
	if (item >= 0)
	{
		if (item < pListBox->scrollPos)
			scrollPos = item;
		else if (item >= pListBox->scrollPos + rcClient.height)
			scrollPos = item - rcClient.height + 1;
	}
	return scrollPos;
}

// Select listbox item
bool listbox_select(LISTBOX* pListBox, int item)
{
	// Get count
	int scrollPos;
	int count = pListBox->getItemCount(pListBox);

	// Check in range
	if (item < 0)
		item = 0;
	if (item >= count)
		item = count - 1;

	// Scroll visible
	scrollPos = listbox_ensure_visible(pListBox, item);

	// Update display
	return listbox_update(pListBox, item, scrollPos);
}


int lb_GetItemCount(LISTBOX* pListBox)
{
	return pListBox->itemCount;
}

const char* lb_GetItemText(LISTBOX* pListBox, int item)
{
	return pListBox->ppItems[item];
}

void listbox_set_data(LISTBOX* pListBox, int itemCount, const char** ppItems)
{
	if (itemCount < 0)
	{
		const char** pp = ppItems;
		itemCount = 0;
		while (*pp)
		{
			itemCount++;
			pp++;
		}
	}
	pListBox->getItemCount = lb_GetItemCount;
	pListBox->getItemText = lb_GetItemText;
	pListBox->itemCount = itemCount;
	pListBox->ppItems = ppItems;
}


size_t listbox_wndproc(WINDOW* pWindow, MSG* pMsg)
{
	LISTBOX* pListBox = (LISTBOX*)pWindow;
	switch (pMsg->message)
	{
		case MESSAGE_DRAWCONTENT:
			listbox_draw(pListBox);
			break;

		case MESSAGE_KEYDOWN:
		{
			int newSel = pListBox->selectedItem;
			switch (pMsg->param1)
			{
				case KEY_UP:
					newSel--;
					break;

				case KEY_DOWN:
					newSel++;
					break;

				case KEY_NEXT:
				{
					RECT rcClient;
					window_get_client_rect(&pListBox->window, &rcClient);
					newSel += rcClient.height;
					break;
				}

				case KEY_PRIOR:
				{
					RECT rcClient;
					window_get_client_rect(&pListBox->window, &rcClient);
					newSel -= rcClient.height;
					break;
				}

				case KEY_HOME:
					newSel = 0;
					break;

				case KEY_END:
					newSel = pListBox->getItemCount(pListBox) - 1;
					break;

				case KEY_ESCAPE:
					if (pListBox->window.modal)
						window_end_modal(-1);
					break;

				case KEY_ENTER:
					if (pListBox->window.modal)
						window_end_modal(pListBox->selectedItem);
					break;

				default:
					return false;
			}

			// Select and redraw
			listbox_select(pListBox, newSel);
		}
	}

	return window_def_proc(pWindow, pMsg);
}
