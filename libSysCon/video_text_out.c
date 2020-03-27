#include "libSysCon.h"
#include <string.h>

void video_text_out(uint8_t x, uint8_t y, const char* psz)
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;
	uint8_t* p = video_char_ram + x + y * SCREEN_WIDTH;
	uint8_t length = strlen(psz);
	memcpy(p, psz, length);
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}

