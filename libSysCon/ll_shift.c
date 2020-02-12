#include "libSysCon.h"

// Remove an item from the head of a singly linked list
void* ll_shift(void** pHead)
{
	LISTITEM* p = (LISTITEM*)* pHead;
	if (!p)
		return NULL;

	*pHead = p->pNext;
	p->pNext = NULL;
	return p;
}
