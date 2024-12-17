#ifndef _utils_h
#define _utils_h

#include <stdbool.h>

#define array_size(x) sizeof(x) / sizeof(x[0])

#define continue_or_retrun(x) ({ if (!x) return false; })
#define continue_break_or_retrun(x) ({ if (!x) return false; break; })

#endif
