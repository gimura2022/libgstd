#include "include/dynarr.h"
#include <stdio.h>
#include <stdlib.h>

#include <gstd/dynarr.h>
#include <gstd/dynarr_malloc.h>
#include <gstd/allocators.h>

int main(int argc, char* argv[])
{
	struct gstd__memmanager memmanager = {
		.allocator   = malloc,
		.deallocator = free,
	};

	struct gstd__dynarr arr = {0};
	gstd__dynarr_malloc_create(&arr, &memmanager, sizeof(int));

	int i = 100;
	gstd__dynarr_push_end(&arr, &i);
	i = 200;
	gstd__dynarr_push_end(&arr, &i);
	i = 300;
	gstd__dynarr_push_end(&arr, &i);

	i = 50;
	gstd__dynarr_push_start(&arr, &i);

	for (int i = 0; i < gstd__dynarr_len(&arr); i++) {
		int* iptr = gstd__dynarr_get(&arr, i);
		printf("%i\n", *iptr);
	}

	gstd__dynarr_free(&arr);

	return 0;
}
