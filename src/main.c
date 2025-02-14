#include "malloc.h"

int main() {
	char *test1 = malloc(5);
	char *test2 = malloc(4095);

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

	ft_printf("MALLOC\n");
	show_alloc_mem();
	ft_printf("\n");
	char *test3 = malloc(10000);
	show_alloc_mem();
	ft_printf("\n");
	char *test4 = malloc(20000);
	show_alloc_mem();
	ft_printf("\n");

	ft_printf("REALLOC\n");
	test2 = realloc(test2, 80);
	show_alloc_mem();
	ft_printf("\n");
	test2 = realloc(test2, 90);
	show_alloc_mem();
	ft_printf("\n");

	ft_printf("FREE\n");
	free(test1);
	free(test2);
	free(test3);
	free(test4);
	show_alloc_mem();
}
