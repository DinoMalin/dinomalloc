#include "malloc.h"

void ft_free(void *addr) {
	if (!addr)
		return;

	data *item = get_item(addr);
	if (!item)
		return;

	if (item->addr != tiny && item->addr != medium)
		munmap(item->addr, item->len);

	item->addr = NULL;
	item->len = 0;
	item->zone = 0;
}
