#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <glog.h>

#include "allocators.h"
#include "dunarr.h"
#include "gstd.h"

void gstd__dunarr_create(struct gstd__dunarr* arr, struct gstd__memmanager* memanager)
{
	bool res = gstd__dunarr_create_with_preallocated_size(arr, GSTD__DUNARR_DEFAULT_PREALLOCATED_SIZE,
			memanager);

	if (!res)
		glog__unreachable(gstd__logger, "default preallocated size is zero (why?)");
}

static struct gstd__dunarr_member* add_member(struct gstd__memmanager* memanager,
		struct gstd__dunarr_member* prev, struct gstd__dunarr_member* next);
static void remove_member(struct gstd__memmanager* memmanager, struct gstd__dunarr_member* member,
		struct gstd__dunarr_member* prev, struct gstd__dunarr_member* next);

bool gstd__dunarr_create_with_preallocated_size(struct gstd__dunarr* arr, size_t size,
		struct gstd__memmanager* memmanager)
{
	if (size == 0)
		return false;

	arr->memmanager = memmanager;
	arr->start     = NULL;
	arr->end       = NULL;

	struct gstd__dunarr_member* cur0 = NULL;
	struct gstd__dunarr_member* cur1 = NULL;
	
	for (size_t i = 0; i < size; i++) {
		struct gstd__dunarr_member* cur = add_member(arr->memmanager, cur0, cur1);	
		cur->data       = NULL;
		cur->is_occuped = false;

		if (cur0 == NULL)
			cur0 = cur;
		else if (cur1 == NULL)
			cur1 = cur;
	}

	arr->start = cur0;
	
	if (cur1 == NULL)
		arr->end = cur0;
	else
		arr->end = cur1;

	return true;
}

static bool dunarr_destroy_enumerator(struct gstd__dunarr_member* member, struct gstd__dunarr* arr,
		void* custom_data);

void gstd__dunarr_destroy(struct gstd__dunarr* arr)
{
	bool res = gstd__dunarr_enumerate_to_end(arr, dunarr_destroy_enumerator, NULL);
	if (!res)
		glog__unreachable(gstd__logger, "destroy enumerator returns false");
}

bool gstd__dunarr_enumerate_to_end(struct gstd__dunarr* arr, gstd__dunarr_enumerator_f func,
		void* custom_data)
{
	struct gstd__dunarr_member *i, *next;
	for (i = arr->start, next = i->next; next != NULL; i = next, next = i->next) {
		if (!func(i, arr, custom_data))
			return false;
	}

	return true;
}

bool gstd__dunarr_enumerate_to_start(struct gstd__dunarr* arr, gstd__dunarr_enumerator_f func,
		void* custom_data)
{
	struct gstd__dunarr_member *i, *prev;
	for (i = arr->end, prev = i->prev; prev != NULL; i = prev, prev = i->prev) {
		if (!func(i, arr, custom_data))
			return false;
	}

	return true;
}

void gstd__dunarr_push_end(struct gstd__dunarr* arr, void* data)
{
	bool res = gstd__dunarr_push_id(arr, gstd__dunarr_len(arr), data);

	if (!res)
		glog__unreachable(gstd__logger, "gived empty array");
}

void gstd__dunarr_push_start(struct gstd__dunarr* arr, void* data)
{
	bool res = gstd__dunarr_push_id(arr, 0, data);

	if (!res)
		glog__unreachable(gstd__logger, "gived empty array");
}

static bool get_member_by_id(const struct gstd__dunarr* arr, size_t id, struct gstd__dunarr_member** member);

bool gstd__dunarr_push_id(struct gstd__dunarr* arr, size_t i, void* data)
{
	struct gstd__dunarr_member* member;
	get_member_by_id(arr, i, &member);

	if (!member->is_occuped) {
		member->is_occuped = true;
		member->data       = data;

		return true;
	}

	struct gstd__dunarr_member* new = add_member(arr->memmanager, member, member->next);
	new->is_occuped = true;
	new->data       = data;

	return true;
}

void gstd__dunarr_pop_end(struct gstd__dunarr* arr)
{
	bool res = gstd__dunarr_pop_id(arr, gstd__dunarr_len(arr));

	if (!res)
		glog__unreachable(gstd__logger, "gived empty array");
}

void gstd__dunarr_pop_start(struct gstd__dunarr* arr)
{
	bool res = gstd__dunarr_pop_id(arr, 0);

	if (!res)
		glog__unreachable(gstd__logger, "gived empty array");
}

bool gstd__dunarr_pop_id(struct gstd__dunarr* arr, size_t id)
{
	struct gstd__dunarr_member* member;
	if (!get_member_by_id(arr, id, &member))
		return false;

	remove_member(arr->memmanager, member, member->prev, member->next);

	return true;
}

bool gstd__dunarr_set(struct gstd__dunarr* arr, size_t i, void* data)
{
	struct gstd__dunarr_member* member;
	if (!get_member_by_id(arr, i, &member))
		return false;

	member->data = data;

	return true;
}

static bool dunarr_len_enumerator(struct gstd__dunarr_member* member, struct gstd__dunarr* arr,
		void* out);

size_t gstd__dunarr_len(const struct gstd__dunarr* arr)
{
	size_t size = 0;
	bool res = gstd__dunarr_enumerate_to_end((struct gstd__dunarr*) arr, dunarr_len_enumerator, &size);

	if (!res)
		glog__unreachable(gstd__logger, "len enumerator returns false");

	return size;
}

void* gstd__dunarr_get(const struct gstd__dunarr* arr, size_t i)
{
	struct gstd__dunarr_member* member;
	if (!get_member_by_id(arr, i, &member))
		return NULL;

	return member->data;
}

static struct gstd__dunarr_member* add_member(struct gstd__memmanager* memmanager,
		struct gstd__dunarr_member* prev, struct gstd__dunarr_member* next)
{
	struct gstd__dunarr_member* new = memmanager->allocator(sizeof(struct gstd__dunarr_member));
	new->next = next;
	new->prev = prev;

	if (next != NULL)
		next->prev = new;

	if (prev != NULL)
		prev->next = new;

	return new;
}

static bool dunarr_destroy_enumerator(struct gstd__dunarr_member* member, struct gstd__dunarr* arr,
		void* custom_data)
{
	arr->memmanager->deallocator(member);
	return true;
}

static bool dunarr_len_enumerator(struct gstd__dunarr_member* member, struct gstd__dunarr* arr,
		void* out)
{
	size_t* out_size = (size_t*) out;
	if (member->is_occuped) (*out_size)++;

	return true;
}

static bool get_member_by_id(const struct gstd__dunarr* arr, size_t id, struct gstd__dunarr_member** member)
{
	int i                           = 0;
	struct gstd__dunarr_member* cur = arr->start;
	bool is_found                   = false;
	bool perm_false                 = false;
	
	for (; cur != NULL; cur = cur->next, i++) {
		if (i == id) {
			is_found = true;
			break;
		}

		if (!cur->is_occuped)
			perm_false = true;
	}

	*member = cur;

	return is_found && !perm_false;
}

static void remove_member(struct gstd__memmanager* memmanager, struct gstd__dunarr_member* member,
		struct gstd__dunarr_member* prev, struct gstd__dunarr_member* next)
{
	prev->next = next;
	next->prev = prev;

	memmanager->deallocator(member);
}
