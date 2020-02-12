#include "libSysCon.h"

// Append an item to the end of a singly linked list
void* ll_push(void** pHead, void* pItem)
{
	((LISTITEM*)pItem)->pNext = NULL;
	if (!*pHead)
	{
		*pHead = pItem;
	}
	else
	{
		LISTITEM* p = (LISTITEM*)*pHead;
		while (p->pNext)
			p = p->pNext;
		p->pNext = pItem;
	}
	return pItem;
}

