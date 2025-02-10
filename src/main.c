#include "malloc.h"

#define TINY getpagesize()
#define MEDIUM getpagesize()*getpagesize()

#define ALLOC(addr, len) \
	mmap(addr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

void *g_allocated[] = {NULL};

void *ft_malloc(size_t len) {
	static void *tiny = ALLOC(NULL, TINY);
	static void *medium = ALLOC(NULL, MEDIUM);

	len += sizeof(size_t);

	if (len <= TINY) {

	}

	size_t *res = ALLOC(len);
	if (res == MAP_FAILED)
		return 0;

	*res = len;
	res += sizeof(size_t);
	return (void*)res;
}

void ft_free(void *addr) {
	if (!addr)
		return;

	addr = addr - sizeof(size_t);
	size_t *len = addr;
	munmap(addr, *len);
}

int main() {
	printf("page: %d\n", getpagesize());
	char *test = ft_malloc(5);

	if (test) {
		test[0] = 'd';
		test[1] = 'i';
		test[2] = 'n';
		test[3] = 'o';
		test[4] = '\0';
		printf("test: [%s]\n", test);
	}

	ft_free(test);
}
