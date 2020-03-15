#include "libSysCon.h"
#include <string.h>

void video_clear()
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;
    memset(video_char_ram, ' ', sizeof(video_char_ram));
    memset(video_color_ram, 0, sizeof(video_color_ram));
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}
