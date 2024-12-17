#include "dynarr.h"
#include "utils.h"

void gstd__dynarr_free(struct gstd__dynarr* arr)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t data       = arr->real_arr_data;

	impl->free(data);
}

void gstd__dynarr_push_end(struct gstd__dynarr* arr, void* data)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	size_t arr_size = impl->len(arr_data);
	gstd__dynarr_push_id(arr, arr_size, data);
}

void gstd__dynarr_push_start(struct gstd__dynarr* arr, void* data)
{
	gstd__dynarr_push_id(arr, 0, data);
}

bool gstd__dynarr_push_id(struct gstd__dynarr* arr, size_t i, void* data)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	continue_or_retrun(impl->push(arr_data, i));
	continue_or_retrun(impl->set(arr_data, i, data));

	return true;
}

void gstd__dynarr_pop_end(struct gstd__dynarr* arr)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	size_t arr_size = impl->len(arr_data);
	gstd__dynarr_pop_id(arr, arr_size);
}

void gstd__dynarr_pop_start(struct gstd__dynarr* arr)
{
	gstd__dynarr_pop_id(arr, 0);
}

bool gstd__dynarr_pop_id(struct gstd__dynarr* arr, size_t i)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	continue_or_retrun(impl->pop(arr_data, i));

	return true;
}

bool gstd__dynarr_set(struct gstd__dynarr* arr, size_t i, void* data)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	continue_or_retrun(impl->set(arr_data, i, data));

	return false;
}

void* gstd__dynarr_get(const struct gstd__dynarr* arr, size_t i)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	return impl->get(arr_data, i);
}

size_t gstd__dynarr_len(const struct gstd__dynarr* arr)
{
	const struct gstd__dynarr_impl* impl = arr->impl;
	gstd__dynarr_array_data_t arr_data   = arr->real_arr_data;

	return impl->len(arr_data);
}
