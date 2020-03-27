#include "libSysCon.h"
#include <string.h>

void video_set_color(uint8_t left, uint8_t top, uint8_t width, uint8_t height, uint8_t attr)
{
	// Calculate starting position
	uint8_t* p = video_color_ram + left + top * SCREEN_WIDTH;

	// Fill
	ApmEnable |= APM_ENABLE_VIDEOBANK;
	for (uint8_t i=0; i<height; i++)
	{
		memset(p, attr, width);
		p += SCREEN_WIDTH;
	}
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}

