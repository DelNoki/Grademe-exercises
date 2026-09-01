/*
Implement a function that reorders the nodes of a linked 
list according to a comparator you receive as a parameter.
*/

#ifndef LIST_H
# define LIST_H

typedef struct ListNode
{
	int			data;
	struct ListNode	*next;
} ListNode;

#endif

//if you want a header #include "list.h"

ListNode	*sort_list(ListNode *lst, int (*cmp)(int, int))
{
	ListNode *b = lst;
	int swap;
	
	while (b->next != (void *)0)
	{
		if (cmp(b->data, b->next->data) == 0)
		{
			swap = b->data;
			b->data = b->next->data;
			b->next->data = swap;
			b = lst;
		}
		else
			b = b->next;
	}
	return (lst);
}
