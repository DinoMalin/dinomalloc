#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include "libft.h"

#define TINY (size_t)getpagesize()
#define MEDIUM (size_t)(getpagesize() * 4)
#define SUMMARY (getpagesize() * 2)

#define tiny summary->addr
#define medium (summary+1)->addr

#define ALLOC(len) \
	mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

#define DISPLAY(item)						\
	ft_printf("0x%p - 0x%p : %u bytes\n",	\
		item.addr,							\
		(void*)(item.addr + item.len),		\
		summary[i].len						\
	);

#define NULL ((void*)0)

typedef enum {
	ztiny,
	zmedium,
	zlarge
} t_zone;

typedef struct {
	t_zone	zone;
	void	*addr;
	size_t	len;
} data;

extern data *summary;

void *ft_malloc(size_t len);
void ft_free(void *addr);
void *ft_realloc(void *addr, size_t len);
void valgrind();

bool is_allocated(void *addr);
data *get_item(void *addr);
