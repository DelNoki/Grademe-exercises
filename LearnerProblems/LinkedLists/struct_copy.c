/*
Write record_with_id, which hands back a copy of src carrying the id you 
were given while the record the caller passed keeps every field it had.
*/

#include <stddef.h>

typedef struct record
{
	int	id;
	int	score;
	char	*name;
} record;

record	record_with_id(record src, int id)
{
	record	out = {0, 0, NULL};
	out.score = src.score;
	out.name = src.name;
	out.id = id;
	return (out);
}
