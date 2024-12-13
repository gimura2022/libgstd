#ifndef _gstd_dunarr_h
#define _gstd_dunarr_h

#include <stddef.h>
#include <stdbool.h>

#include <gstd/allocators.h>

#define GSTD__DUNARR_DEFAULT_PREALLOCATED_SIZE 8

struct gstd__dunarr_member {
	void* data;
	bool is_occuped;

	struct gstd__dunarr_member *next, *prev;
};

struct gstd__dunarr {
	struct gstd__memmanager* memmanager;
	struct gstd__dunarr_member *start, *end;
};

void gstd__dunarr_create(struct gstd__dunarr* arr, struct gstd__memmanager* memanager);
bool gstd__dunarr_create_with_preallocated_size(struct gstd__dunarr* arr, size_t size,
		struct gstd__memmanager* memmanager);

void gstd__dunarr_destroy(struct gstd__dunarr* arr);

typedef bool (*gstd__dunarr_enumerator_f)(struct gstd__dunarr_member* member, struct gstd__dunarr* arr,
		void* custom_data);
bool gstd__dunarr_enumerate_to_end(struct gstd__dunarr* arr, gstd__dunarr_enumerator_f func,
		void* custom_data);
bool gstd__dunarr_enumerate_to_start(struct gstd__dunarr* arr, gstd__dunarr_enumerator_f func,
		void* custom_data);

void gstd__dunarr_push_end(struct gstd__dunarr* arr, void* data);
void gstd__dunarr_push_start(struct gstd__dunarr* arr, void* data);
bool gstd__dunarr_push_id(struct gstd__dunarr* arr, size_t i, void* data);

void gstd__dunarr_pop_end(struct gstd__dunarr* arr);
void gstd__dunarr_pop_start(struct gstd__dunarr* arr);
bool gstd__dunarr_pop_id(struct gstd__dunarr* arr, size_t id);

bool gstd__dunarr_set(struct gstd__dunarr* arr, size_t i, void* data);
size_t gstd__dunarr_len(const struct gstd__dunarr* arr);
void* gstd__dunarr_get(const struct gstd__dunarr* arr, size_t i);

#endif
