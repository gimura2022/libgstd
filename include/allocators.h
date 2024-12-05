#ifndef _allocators_h
#define _allocators_h

#include <stddef.h>

typedef void *(*gstd__allocator_t)(size_t);
typedef void  (*gstd__deallocator_t)(void*);

struct gstd__memmanager {
	gstd__allocator_t allocator;
	gstd__deallocator_t deallocator;
};

#endif
