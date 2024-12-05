#ifndef _strref_h
#define _strref_h

#include <stddef.h>
#include <stdbool.h>

struct gstd__strref {
	const char* start;
	const char* end;

	struct gstd__strref* next;
};

struct gstd__strref gstd__strref_from_str(const char* str);
void gstd__str_from_strref(char* buf, const struct gstd__strref* strref);

bool gstd__strref_cmp(const struct gstd__strref* str0, const struct gstd__strref* str1);
void gstd__strref_cat(struct gstd__strref* dest, struct gstd__strref* src);
size_t gstd__strref_len(const struct gstd__strref* str);

#endif
