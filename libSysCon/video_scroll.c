#include "libSysCon.h"
#include <string.h>

void video_scroll(uint8_t left, uint8_t top, uint8_t width, uint8_t height, 
				int dy, bool attr,
				uint8_t* pRedrawFrom, uint8_t* pRedrawCount)
{
	// Redundant
	if (dy==0)
	{
		*pRedrawFrom = 0;
		*pRedrawCount = 0;
		return;
	}

	uint8_t first, last;
	int delta;

	if (dy < 0)
	{
		// Scroll up
		if (-dy > height)
		{
			*pRedrawFrom = 0;
			*pRedrawCount = height;
			return;
		}

		first = top - dy;
		last = top + height;
		delta = 1;
		*pRedrawFrom = height + dy;
		*pRedrawCount = -dy;
	}
	else
	{
		// Scroll down
		if (dy > height)
		{
			*pRedrawFrom = 0;
			*pRedrawCount = height;
			return;
		}

		last = top - 1;
		first = top + height - dy - 1;
		delta = -1;
		*pRedrawFrom = 0;
		*pRedrawCount = dy;
	}

	ApmEnable |= APM_ENABLE_VIDEOBANK;
	for (uint8_t i=first; i!=last; i+=delta)
	{
		char* pSrc = video_char_ram + i * SCREEN_WIDTH + left;
		char* pDest = video_char_ram + (i + dy) * SCREEN_WIDTH + left;

		// Move characters
		memcpy(pDest, pSrc, width);

		// Move attributes
		if (attr)
		{
			memcpy(pDest + (video_color_ram - video_char_ram),
					pSrc + (video_color_ram - video_char_ram), 
					width);
		}
	}
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}

