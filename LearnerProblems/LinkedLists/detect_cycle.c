/*
Implement a function that decides whether a singly linked list 
eventually loops back onto itself or terminates with a NULL pointer.
*/

#ifndef LIST_H
# define LIST_H

typedef struct ListNode
{
	void          *data;
	struct ListNode *next;
} ListNode;

#endif

//#include "list.h"
#include <stddef.h>

int	detect_cycle(ListNode *list)
{
	ListNode *node = list;
	ListNode *visited;
	int count = 0;
	int j;
	while (node != NULL)
	{
		visited = list;
		j = 0;
		while (visited != NULL && j < count)
		{
			if (visited == node)
				return (1);
			visited = visited->next;
			j++;
		}
		count++;
		node = node->next;
	}
	return (0);
}

/*
Alternative : si on est sur un cercle, alors un pointeur qui se déplace à une vitesse 2 
finira par rattraper un pointeur qui se déplace à une vitesse de 1. (très optimal !!!)
int detect_cycle(ListNode *list)
{
    ListNode *slow;
    ListNode *fast;

    if (list == NULL)
        return 0;

    slow = list;
    fast = list;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return 1;
    }
    return 0;
}
*/