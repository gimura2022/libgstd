#ifndef _gstd_dynarr_malloc_h
#define _gstd_dynarr_malloc_h

#include "dynarr.h"
#include "allocators.h"

void gstd__dynarr_malloc_create(struct gstd__dynarr* arr, struct gstd__memmanager* memmanager,
		size_t element_size);

#endif
