#include "malloc.h"

#define tiny summary->addr
#define medium (summary+1)->addr
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

bool is_allocated(void *addr) {
	int t = SUMMARY/sizeof(data);
	for (int i = 2; i < t; i++) {
		if (!summary[i].addr)
			continue;
		if (addr >= summary[i].addr && addr <= summary[i].addr + summary[i].len) {
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
			curr_len+=sizeof(void*);
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


data *get_item(void *addr) {
	int t = SUMMARY/sizeof(data);
	for (int i = 2; i < t; i++) {
		if (addr == summary[i].addr) {
			return summary+i;
		}
	}
	return NULL;
}

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

void valgrind() {
	int t = SUMMARY/sizeof(data);
	size_t total = 0;

	ft_printf("TINY : 0x%p\n", tiny);
	for (int i = 2; i < t; i++) {
		if (summary[i].zone == ztiny && summary[i].addr) {
			DISPLAY(summary[i]);
			total += summary[i].len;
		}
	}

	ft_printf("MEDIUM : 0x%p\n", medium);
	for (int i = 2; i < t; i++) {
		if (summary[i].zone == zmedium && summary[i].addr) {
			DISPLAY(summary[i]);
			total += summary[i].len;
		}
	}

	ft_printf("LARGE : everything else\n");
	for (int i = 2; i < t; i++) {
		if (summary[i].zone == zlarge && summary[i].addr) {
			DISPLAY(summary[i]);
			total += summary[i].len;
		}
	}

	ft_printf("TOTAL : %u bytes\n", total);
}

int main() {
	char *test1 = ft_malloc(5);
	char *test2 = ft_malloc(4095);

	if (test1) {
		test1[0] = 'd';
		test1[1] = 'i';
		test1[2] = 'n';
		test1[3] = 'o';
		test1[4] = '\0';
		ft_printf("test: [%s]\n", test1);
	}

	if (test2) {
		test2[4092] = '4';
		test2[4093] = '2';
		test2[4094] = '\0';
		ft_printf("test: [%s]\n", &test2[4092]);
	}

	ft_printf("\n");
	valgrind();
	ft_printf("\n");
	char *test3 = ft_malloc(10000);
	valgrind();
	ft_printf("\n");
	char *test4 = ft_malloc(20000);
	valgrind();
	ft_printf("\n");
	ft_free(test1);
	ft_free(test2);
	ft_free(test3);
	ft_free(test4);
	ft_printf("\n");
	valgrind();
}
