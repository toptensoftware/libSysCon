#include "libSysCon.h"
#include <string.h>

char* video_char_ram = video_char_ram_hw;
char* video_color_ram = video_color_ram_hw;

__at(0xF800) char video_char_ram_offscreen[SCREEN_WIDTH * SCREEN_HEIGHT];
__at(0xFA00) char video_color_ram_offscreen[SCREEN_WIDTH * SCREEN_HEIGHT];

uint8_t g_offscreen_depth = 0;

void video_begin_offscreen()
{
	if (g_offscreen_depth == 0)
	{
		ApmEnable |= APM_ENABLE_VIDEOBANK;
		memcpy(video_char_ram_offscreen, video_char_ram_hw, 0x400);
		video_char_ram = video_char_ram_offscreen;
		video_color_ram = video_color_ram_offscreen;
		ApmEnable &= ~APM_ENABLE_VIDEOBANK;
	}
	g_offscreen_depth++;
}

void video_end_offscreen()
{
	if (g_offscreen_depth == 1)
	{
		ApmEnable |= APM_ENABLE_VIDEOBANK;
		memcpy(video_char_ram_hw, video_char_ram_offscreen, 0x400);
		video_char_ram = video_char_ram_hw;
		video_color_ram = video_color_ram_hw;
		ApmEnable &= ~APM_ENABLE_VIDEOBANK;
	}
	g_offscreen_depth--;
}
