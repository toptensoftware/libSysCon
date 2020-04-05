#include "libSysCon.h"
#include <string.h>

const char* stralloc(const char* psz)
{
	if (psz == NULL)
		return psz;

	// Calculate length
	int len = strlen(psz);

	// Allocate memory and copy
	char* pszNew = malloc(len + 1);
	strcpy(pszNew, psz);

	// Done
	return pszNew;
}
