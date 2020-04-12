#include "libSysCon.h"
#include <string.h>

void video_clear()
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;
    memset(video_char_ram, ' ', SCREEN_WIDTH * SCREEN_HEIGHT);
    memset(video_color_ram, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}
