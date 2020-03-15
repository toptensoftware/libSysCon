#include "libSysCon.h"
#include <string.h>

void video_write_sz(uint8_t x, uint8_t y, const char* psz, uint8_t color)
{
	video_write(x, y, psz, strlen(psz), color);
}
