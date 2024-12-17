#ifndef _gstd_dynarr_h
#define _gstd_dynarr_h

#include <stddef.h>
#include <stdbool.h>

#include <gstd/allocators.h>

typedef void* gstd__dynarr_array_data_t;

typedef void* (*gstd__dynarr_impl_get_f)(gstd__dynarr_array_data_t, size_t);
typedef bool (*gstd__dynarr_impl_set_f)(gstd__dynarr_array_data_t, size_t, void*);
typedef bool (*gstd__dynarr_impl_push_f)(gstd__dynarr_array_data_t, size_t);
typedef bool (*gstd__dynarr_impl_pop_f)(gstd__dynarr_array_data_t, size_t);
typedef size_t (*gstd__dynarr_impl_len_f)(gstd__dynarr_array_data_t);
typedef void (*gstd__dynarr_impl_free_f)(gstd__dynarr_array_data_t);

struct gstd__dynarr_impl {
	gstd__dynarr_impl_set_f set;
	gstd__dynarr_impl_get_f get;
	gstd__dynarr_impl_push_f push;
	gstd__dynarr_impl_pop_f pop;
	gstd__dynarr_impl_len_f len;
	gstd__dynarr_impl_free_f free;
};

struct gstd__dynarr {
	gstd__dynarr_array_data_t real_arr_data;
	struct gstd__dynarr_impl* impl;
};

void gstd__dynarr_free(struct gstd__dynarr* arr);

void gstd__dynarr_push_end(struct gstd__dynarr* arr, void* data);
void gstd__dynarr_push_start(struct gstd__dynarr* arr, void* data);
bool gstd__dynarr_push_id(struct gstd__dynarr* arr, size_t i, void* data);

void gstd__dynarr_pop_end(struct gstd__dynarr* arr);
void gstd__dynarr_pop_start(struct gstd__dynarr* arr);
bool gstd__dynarr_pop_id(struct gstd__dynarr* arr, size_t i);

bool gstd__dynarr_set(struct gstd__dynarr* arr, size_t i, void* data);
void* gstd__dynarr_get(const struct gstd__dynarr* arr, size_t i);
size_t gstd__dynarr_len(const struct gstd__dynarr* arr);

#endif
