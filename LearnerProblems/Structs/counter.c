/*
Make counter.h safe to include more than once, 
then implement the three functions it declares.
*/

#ifndef COUNTER_H
#define COUNTER_H

typedef struct s_counter
{
	int	value;
}	counter;

void	counter_reset(counter *item);
void	counter_add(counter *item, int amount);
int	counter_value(const counter *item);

#endif

//#include "counter.h"

void	counter_reset(counter *item)
{
	item->value = 0;
}

void	counter_add(counter *item, int amount)
{
	item->value += amount;
}

int	counter_value(const counter *item)
{
	return ((int)(item->value));
}