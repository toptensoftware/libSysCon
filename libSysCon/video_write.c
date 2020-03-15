#include "libSysCon.h"
#include <string.h>

void video_write(uint8_t x, uint8_t y, const char* psz, uint8_t length, uint8_t color)
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;
    memcpy(video_char_ram + x + y * SCREEN_WIDTH, psz, length);
	memset(video_color_ram + x + y * SCREEN_WIDTH, color, length); 
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}
