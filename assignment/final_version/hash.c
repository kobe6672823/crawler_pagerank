#include"hash.h"
#include <string.h>

unsigned int JSHash(char *str, int prime)
{
	int i;
	int hash = 1315423911;
	int len = strlen(str);

	for(i = 0; i < len; i++)
	{
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}
	hash = hash & 0x7FFFFFFF;
	hash = hash>0?hash:(-1 * hash);
	return (hash % prime);
}

