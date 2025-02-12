#include "malloc.h"

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

data *get_item(void *addr) {
	int t = SUMMARY/sizeof(data);
	for (int i = 2; i < t; i++) {
		if (addr == summary[i].addr) {
			return summary+i;
		}
	}
	return NULL;
}
