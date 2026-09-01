/*
Implement a function that walks a singly linked list and discards every 
node whose payload matches a reference value, freeing the removed nodes as it goes.
*/

#ifndef LIST_H
# define LIST_H

typedef struct ListNode
{
	void			*data;
	struct ListNode	*next;
} ListNode;

#endif

//if header #include "list.h"
#include <stdlib.h>

void	list_filter(ListNode **begin, void *ref, int (*cmp)(void *, void *))
{
	ListNode *node = *begin;
	ListNode *toDelete;
	if (node == NULL)
		return;
	while (node != NULL)
	{
		if (node == *begin && cmp(node->data, ref) == 0)
		{
			toDelete = node;
			*begin = node->next;
			node = *begin;
			free(toDelete);
		}
		else if (node->next != NULL && cmp(node->next->data, ref) == 0)
		{
			toDelete = node->next;
			node->next = node->next->next;
			free(toDelete);
		}
		else
			node = node->next;
	}
}
