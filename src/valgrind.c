#include "malloc.h"

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
