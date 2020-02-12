#include "libSysCon.h"

// Prepend an item at the head of a singly linked list
void* ll_unshift(void** pHead, void* pItem)
{
	((LISTITEM*)pItem)->pNext = *pHead;
	*pHead = pItem;
	return pItem;
}
