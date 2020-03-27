#include "libSysCon.h"
#include <string.h>

void video_draw_box_indirect(RECT* prc, uint8_t attr)
{
	video_draw_box(prc->left, prc->top, prc->width, prc->height, attr);
}

void video_draw_box(uint8_t left, uint8_t top, uint8_t width, uint8_t height, uint8_t attr)
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;

	// Calculate starting position
	uint8_t* p = video_char_ram + left + top * SCREEN_WIDTH;

	// Top Edge
	*p = BOX_TL;
	memset(p + 1, BOX_H, width - 2);
	*(p + width - 1) = BOX_TR;
	memset(p + 0x200, attr, width);
	p += SCREEN_WIDTH;

	// Middle
	for (uint8_t i=0; i<height - 2; i++)
	{
		*p = BOX_V;
		memset(p + 1, ' ', width - 2);
		*(p + width - 1) = BOX_V;
		memset(p + 0x200, attr, width);
		p += SCREEN_WIDTH;
	}

	// Top Edge
	*p = BOX_BL;
	memset(p + 1, BOX_H, width - 2);
	*(p + width - 1) = BOX_BR;
	memset(p + 0x200, attr, width);

	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}
