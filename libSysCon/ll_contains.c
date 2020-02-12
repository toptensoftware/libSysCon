#include "libSysCon.h"

// Check if a list contains an item
bool ll_contains(void* pHead, void* pItem)
{
	LISTITEM* p = (LISTITEM*)pHead;
	if (!p)
		return false;

	while (p)
	{
		if (p == pItem)
			return true;
		p = p->pNext;
	}

	return false;
}
