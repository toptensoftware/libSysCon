#include "libSysCon.h"
#include <string.h>

void video_draw_multiline_text(const char* psz, POINT* pPos)
{
	ApmEnable |= APM_ENABLE_VIDEOBANK;

	int y = pPos->y;
	char* pDest = video_char_ram + pPos->x + y * SCREEN_WIDTH;
	while (psz[0])
	{
		if (psz[0]=='\n')
		{
			y++;
			pDest = video_char_ram + pPos->x + y * SCREEN_WIDTH;
		}
		else
		{
			*pDest = *psz;
			pDest++;
		}
		psz++;
	}

	ApmEnable &= ~APM_ENABLE_VIDEOBANK;
}