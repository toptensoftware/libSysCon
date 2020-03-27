#include "libSysCon.h"
#include <string.h>

void video_measure_multiline_text(const char* psz, POINT* pVal)
{
	int lineWidth = 0;
	pVal->x = 0;
	pVal->y = 0;


	while (psz[0])
	{
		if (psz[0]=='\n')
		{
			if (lineWidth > pVal->x)
				pVal->x = lineWidth;
			lineWidth = 0;
		}
		else
		{
			if (lineWidth == 0)
				pVal->y++;
			lineWidth++;
		}

		psz++;
	}

	if (lineWidth > pVal->x)
		pVal->x = lineWidth;
}
