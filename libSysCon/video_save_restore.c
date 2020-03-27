#include "libSysCon.h"
#include <string.h>

typedef struct tagSAVEDATA
{
	uint8_t left;
	uint8_t top;
	uint8_t width;
	uint8_t height;
	uint8_t data[];
} SAVEDATA;

void copy_screen_data(
	void* baseSrc, uint8_t strideSrc, 
	void* baseDest, uint8_t strideDest, 
	uint8_t leftSrc, uint8_t topSrc, uint8_t leftDest, uint8_t topDest,
	uint8_t width, uint8_t height)
{
	uint8_t y;
	uint8_t* pSrc = ((uint8_t*)baseSrc) + leftSrc + topSrc * strideSrc;
	uint8_t* pDest = ((uint8_t*)baseDest) + leftDest + topDest * strideDest;

	for (y=0; y<height; y++)
	{
		memcpy(pDest, pSrc, width);
		pSrc += strideSrc;
		pDest += strideDest;
	}
}


void* video_save(uint8_t left, uint8_t top, uint8_t width, uint8_t height)
{
	SAVEDATA* sd;

	sd = (SAVEDATA*)malloc(sizeof(SAVEDATA) + width * height * 2);
	sd->left = left;
	sd->top = top;
	sd->width = width;
	sd->height = height;
	ApmEnable |= APM_ENABLE_VIDEOBANK;
	copy_screen_data(video_char_ram, SCREEN_WIDTH, sd->data, width, left, top, 0, 0, width, height);
	copy_screen_data(video_color_ram, SCREEN_WIDTH, sd->data + width * height, width, left, top, 0, 0, width, height);
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
	return sd;
}

void video_restore(void* saveData)
{
	SAVEDATA* sd = (SAVEDATA*)saveData;

	ApmEnable |= APM_ENABLE_VIDEOBANK;
	copy_screen_data(sd->data, sd->width, video_char_ram, SCREEN_WIDTH, 0, 0, sd->left, sd->top, sd->width, sd->height);
	copy_screen_data(sd->data + sd->width * sd->height, sd->width, video_color_ram, SCREEN_WIDTH, 0, 0, sd->left, sd->top, sd->width, sd->height);
	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
	free(sd);
}

