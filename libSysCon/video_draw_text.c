#include "libSysCon.h"
#include <string.h>

void video_draw_text(uint8_t x, uint8_t y, uint8_t width, const char* psz, uint8_t flags)
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;

	// Measure
	uint8_t leadSpace = 0;
	uint8_t tailSpace = 0;
	uint8_t* p = video_char_ram + x + y * SCREEN_WIDTH;
	uint8_t length = strlen(psz);

	// Work out alignment
	if (length > width)
	{
		length = width;
	}
	else
	{
		switch (flags & DT_ALIGNMASK)
		{
			case DT_RIGHT:
				leadSpace = width - length;
				break;

			case DT_CENTER:
				leadSpace = (width - length) / 2;
				break;
		}
	}

	// Leading space
	if ((flags & DT_NOFILL)==0 && leadSpace)
	{
		memset(p, ' ', leadSpace);
	}
	p += leadSpace;

	// Text
	memcpy(p, psz, length);
	p += length;

	// Tail space
	tailSpace = width - (length + leadSpace);
	if ((flags & DT_NOFILL)==0 && tailSpace)
	{
		memset(p, ' ', tailSpace);
	}

	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}
