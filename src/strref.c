#include <stdbool.h>
#include <string.h>

#include <gstd/strref.h>

struct gstd__strref gstd__strref_from_str(const char* str)
{
	return (struct gstd__strref) {
		.start = str,
		.end   = str + strlen(str),
		.next  = NULL,
	};
}

void gstd__str_from_strref(char* buf, const struct gstd__strref* strref)
{
	const char* c = strref->start;
	while (c != strref->end) {
		buf[c - strref->start] = *c;
		c++;
	}

	if (strref->next != NULL)
		gstd__str_from_strref(buf, strref->next);
}

bool gstd__strref_cmp(const struct gstd__strref* str0, const struct gstd__strref* str1)
{
	if (gstd__strref_len(str0) != gstd__strref_len(str1))
		return true;

	const struct gstd__strref* cur_str0 = str0;
	const struct gstd__strref* cur_str1 = str1;

	const char* c0 = cur_str0->start;
	const char* c1 = cur_str1->start;
	while (true) {
		if (*c0 != *c1)
			return true;

		c0++;
		c1++;

		if (c0 == cur_str0->end && cur_str0->next == NULL)
			return false;
		if (c1 == cur_str1->end && cur_str1->next == NULL)
			return false;

		if (c0 == cur_str0->end) {
			cur_str0 = cur_str0->next;
			c0       = cur_str0->start;
		}
		if (c1 == cur_str1->end) {
			cur_str1 = cur_str1->next;
			c1       = cur_str1->start;
		}
	}

	return false;
}

void gstd__strref_cat(struct gstd__strref* dest, struct gstd__strref* src)
{
	dest->next = src;
}

size_t gstd__strref_len(const struct gstd__strref* str)
{
	return (str->end - str->start) + (str->next == NULL ? 0 : gstd__strref_len(str->next));
}
