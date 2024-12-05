#ifndef _allocators_h
#define _allocators_h

#include <stddef.h>

typedef void *(*gstd__allocator)(size_t);
typedef void  (*gstd__deallocator)(void*);

#endif
