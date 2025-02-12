#include "malloc.h"

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

	ft_printf("MALLOC\n");
	valgrind();
	ft_printf("\n");
	char *test3 = ft_malloc(10000);
	valgrind();
	ft_printf("\n");
	char *test4 = ft_malloc(20000);
	valgrind();
	ft_printf("\n");

	ft_printf("REALLOC\n");
	test2 = ft_realloc(test2, 80);
	valgrind();
	ft_printf("\n");
	test2 = ft_realloc(test2, 90);
	valgrind();
	ft_printf("\n");

	ft_printf("FREE\n");
	ft_free(test1);
	ft_free(test2);
	ft_free(test3);
	ft_free(test4);
	valgrind();
}
