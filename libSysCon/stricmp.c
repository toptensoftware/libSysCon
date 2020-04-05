#include "libSysCon.h"

int stricmp(const char* p1, const char* p2)
{
	while (*p1 && *p2)
	{
		char ch1 = tolower(*p1++);
		char ch2 = tolower(*p2++);
		if (ch1 < ch2)
			return -1;
		if (ch1 > ch2)
			return 1;
	}

	if (*p1)
		return 1;
	if (*p2)
		return -1;
	return 0;
}
