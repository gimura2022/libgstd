#include "gstd.h"

struct glog__logger* gstd__logger = NULL;

void gstd__init(struct glog__logger* logger)
{
	gstd__logger = logger;
}
