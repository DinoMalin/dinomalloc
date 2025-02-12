#include "malloc.h"

data *summary = NULL;

bool init_zones() {
	if (summary == NULL) {
		summary = ALLOC(SUMMARY);
		if (summary != MAP_FAILED)
			ft_bzero(summary, SUMMARY);
		else {
			summary = NULL;
			return NULL;
		}
	}
	if (tiny == NULL) {
		tiny = ALLOC(TINY);
		if (tiny == MAP_FAILED) {
			tiny = NULL;
			return false;
		}
	}
	if (medium == NULL) {
		medium = ALLOC(MEDIUM);
		if (medium == MAP_FAILED) {
			medium = NULL;
			return false;
		}
	}
	return true;
}

bool add(data *item) {
	int t = SUMMARY/sizeof(data);
	for (int i = 2; i < t; i++) {
		if (!summary[i].addr) {
			summary[i] = *item;
			return true;
		}
	}
	return false;
}

void *get_hole(size_t len, void *zzone, size_t zone_size) {
	void **v_zone = (void **)zzone;
	for (size_t i = 0; i+1 < zone_size; i++) {
		if (len + i >= zone_size) {
			return NULL;
		}

		size_t j = 0;
		size_t curr_len = 0;
		while (!is_allocated(v_zone+i+j)) {
			j++;
			curr_len += sizeof(void*);
			if (curr_len >= len) {
				return v_zone+i;
			}
	  	}
	}
	return NULL;
}

void get_addr(data *item) {
	if (item->zone == zlarge)
		item->addr = ALLOC(item->len);

	if (item->zone == ztiny) {
		item->addr = get_hole(item->len, tiny, TINY);
		if (!item->addr) {
			item->zone = zmedium;
			get_addr(item);
		}
	} else if (item->zone == zmedium) {
		item->addr = get_hole(item->len, medium, MEDIUM);
		if (!item->addr) {
			item->zone = zlarge;
			get_addr(item);
		}
	}
}

void *ft_malloc(size_t len) {
	if (!init_zones()) {
		return NULL;
	}
	data item = {
		.len = len,
		.zone =	len <= TINY		? ztiny		:
				len <= MEDIUM	? zmedium	:
								  zlarge,
	};
	get_addr(&item);
	if (!add(&item)) {
		return NULL;
	}
	return item.addr;
}
