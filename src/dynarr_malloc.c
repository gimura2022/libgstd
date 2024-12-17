#include "dynarr.h"
#include "dynarr_malloc.h"
#include "allocators.h"
#include <string.h>

struct arr_data {
	struct gstd__memmanager* memmanager;

	void* data;
	size_t data_size;
	size_t element_size;
};

static void impl_free(gstd__dynarr_array_data_t _data);
static void* impl_get(gstd__dynarr_array_data_t _data, size_t i);
static size_t impl_len(gstd__dynarr_array_data_t _data);
static bool impl_pop(gstd__dynarr_array_data_t _data, size_t i);
static bool impl_push(gstd__dynarr_array_data_t _data, size_t i);
static bool impl_set(gstd__dynarr_array_data_t _data, size_t i, void* element);

static const struct gstd__dynarr_impl impl = {
	.free = impl_free,
	.get  = impl_get,
	.len  = impl_len,
	.pop  = impl_pop,
	.push = impl_push,
	.set  = impl_set,
};

void gstd__dynarr_malloc_create(struct gstd__dynarr* arr, struct gstd__memmanager* memmanager,
		size_t element_size)
{
	arr->impl = &impl;

	struct arr_data data = {
		.memmanager   = memmanager,
		.element_size = element_size,
		.data         = NULL,
		.data_size    = 0,
	};

	arr->real_arr_data = memmanager->allocator(sizeof(struct arr_data));
	*((struct arr_data*) arr->real_arr_data) = data;
}

static void impl_free(gstd__dynarr_array_data_t _data)
{
	struct arr_data* data = (struct arr_data*) _data;

	if (data->data != NULL)
		data->memmanager->deallocator(data->data);

	data->memmanager->deallocator(data);
}

static void* impl_get(gstd__dynarr_array_data_t _data, size_t i)
{
	struct arr_data* data = (struct arr_data*) _data;

	if (data->data == NULL)
		return NULL;

	if (data->data_size <= i)
		return NULL;

	return data->data + (i * data->element_size);
}

static size_t impl_len(gstd__dynarr_array_data_t _data)
{
	struct arr_data* data = (struct arr_data*) _data;

	return data->data_size;
}

static bool impl_pop(gstd__dynarr_array_data_t _data, size_t i)
{
	struct arr_data* data = (struct arr_data*) _data;

	if (i >= data->data_size)
		return false;

	size_t new_data_size = data->data_size - 1;
	void* new_data       = data->memmanager->allocator(new_data_size * data->element_size);

	memcpy(new_data, data->data, i * data->element_size);
	memcpy(new_data + data->element_size, data->data + data->element_size,
			i * data->element_size);

	if (data->data != NULL)
		data->memmanager->deallocator(data->data);

	data->data      = new_data;
	data->data_size = new_data_size;

	return true;
}

static bool impl_push(gstd__dynarr_array_data_t _data, size_t i)
{
	struct arr_data* data = (struct arr_data*) _data;

	if (i > data->data_size)
		return false;

	size_t new_data_size = data->data_size + 1;
	void* new_data       = data->memmanager->allocator(new_data_size * data->element_size);

	if (data->data != NULL) {
		memcpy(new_data, data->data, i * data->element_size);
		memcpy(new_data + (data->element_size * (i + 1)), data->data + data->element_size * i,
				(data->data_size - i) * data->element_size);

		data->memmanager->deallocator(data->data);
	}

	data->data      = new_data;
	data->data_size = new_data_size;

	return true;
}

static bool impl_set(gstd__dynarr_array_data_t _data, size_t i, void* element)
{
	struct arr_data* data = (struct arr_data*) _data;

	if (i >= data->data_size)
		return false;

	memcpy(data->data + (i * data->element_size), element, data->element_size);

	return true;
}
