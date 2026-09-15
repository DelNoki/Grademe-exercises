/*
Declare the four fields of record in the order that brings sizeof(record) down to 16 bytes, 
then implement the accessors Grademe uses to read them back.
*/

/* record.h */
#ifndef RECORD_H
# define RECORD_H

typedef struct s_record
{
	unsigned char	flag;
	short		code;
	int		id;
	long		stamp;
}	record;

void		record_set(record *dst, long stamp, int id, short code,unsigned char flag);
long		record_stamp(const record *src);
int		record_id(const record *src);
short		record_code(const record *src);
unsigned char	record_flag(const record *src);

#endif

//#include "record.h"

void	record_set(record *dst, long stamp, int id, short code,unsigned char flag)
{
	if (!dst)
		return;
	dst->flag = flag;
	dst->code = code;
	dst->id = id;
	dst->stamp = stamp;
}

long	record_stamp(const record *src)
{
	return ((long)(src->stamp));
}

int	record_id(const record *src)
{
	return ((int)(src->id));
}

short	record_code(const record *src)
{
	return ((short)(src->code));
}

unsigned char	record_flag(const record *src)
{
	return ((unsigned char)(src->flag));
}