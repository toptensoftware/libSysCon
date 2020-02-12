#include "libSysCon.h"

// Remove an item from anywhere in the list
void* ll_remove(void** pHead, void* pItem)
{
	LISTITEM* p = (LISTITEM*)* pHead;
	if (!p)
		return NULL;

	LISTITEM* pPrev = NULL;
	while (p != pItem)
	{
		pPrev = p;
		p = p->pNext;
	}

	if (pPrev)
		pPrev->pNext = p->pNext;
	else
		*pHead = p->pNext;

	return pItem;
}