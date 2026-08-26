//#include "list.h"
#ifndef LIST_H
# define LIST_H

typedef struct ListNode
{
	void			*data;
	struct ListNode	*next;
} ListNode;

#endif

#include <stddef.h>

void	list_apply(ListNode *list, void (*f)(void *))
{
	if (!list || !f)
		return ;
	while (list != NULL)
	{
		f(list->data);
		list = list->next;
	}
}