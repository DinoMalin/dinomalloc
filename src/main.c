#include "malloc.h"

#define TINY (size_t)getpagesize()
#define MEDIUM (size_t)(getpagesize() * getpagesize())
#define SUMMARY (getpagesize() * 2)

#define PROT PROT_READ | PROT_WRITE
#define MAP MAP_PRIVATE | MAP_ANONYMOUS

#define ALLOC(addr, len) \
	mmap(addr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

#define CASTV(a) ((void**)a)
#define CAST(a) ((int*)a)
#define RETURN(res, len)							\
	{												\
		if (res) {									\
			if (add_to_summary(res, len) == -1)		\
				return NULL;						\
			return res;								\
		}											\
	}

#define NULL ((void*)0)

void *summary = NULL;

int get_size(void *addr) {
	for (int i = 0; i < SUMMARY; i+=2) {
		void *this_addr = CASTV(summary+i);
		int size = *CAST(summary+i+1);
		if (addr >= this_addr && addr <= this_addr + size)
			return size;
	}
	return 0;
}

void *find_available_size(void *start, int zone, int target_size) {
	int size = 0;
	for (int i = 0; i < zone; i++) {
		if (get_size(&start[i])) {
			size = 0;
			continue;
		}
		size++;
		if (size == target_size)
			return &start[i];
	}
	return NULL;
}

int add_to_summary(void *addr, int size) {
	for (int i = 0; i < SUMMARY; i+=2) {
		if (!*CAST(summary+i)) {
			*CASTV(summary+i) = addr;
			*CAST(summary+i+1) = size;
			return 1;
		}
	}
	return -1;
}

void *ft_malloc(size_t len) {
	static void *tiny = NULL;
	static void *medium = NULL;

	if (!tiny || !medium) {
		tiny = ALLOC(NULL, TINY);
		medium = ALLOC(NULL, MEDIUM);
		summary = ALLOC(NULL, SUMMARY);
	}
	if (tiny == MAP_FAILED || medium == MAP_FAILED || summary == MAP_FAILED)
		return NULL;
	if (!len)
		return NULL;

	len += sizeof(size_t);

	if (len <= TINY) {
		void *res = find_available_size(tiny, TINY, len);
		RETURN(res, len);
	}
	if (len <= MEDIUM) {
		void *res = find_available_size(medium, MEDIUM, len);
		RETURN(res, len);
	}

	void *res = ALLOC(NULL, len);
	if (res == MAP_FAILED)
		return NULL;
	RETURN(res, len);
	return NULL;
}

void ft_free(void *addr) {
	if (addr)
		munmap(addr, get_size(addr));
}

int main() {
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
