#include "malloc.h"

void *realloc(void *addr, size_t len) {
	data *item = get_item(addr);
	if (len == item->len)
		return addr;

	size_t zone_size =	item->zone == ztiny		? TINY :
						item->zone == zmedium	? MEDIUM :
												  0;
	void *zzone =	item->zone == ztiny		? tiny :
					item->zone == zmedium	? medium :
											  NULL;
	size_t i = 0;
	size_t curr_len = item->len;
	if (item->zone != zlarge && len > item->len) {
		while (!is_allocated(item->addr+i) && item->addr+i < zzone+zone_size) {
			i++;
			curr_len += sizeof(void*);
			if (curr_len >= len) {
				item->len = len;
				return addr;
			}
		}
	}

	free(addr);
	return malloc(len);
}
