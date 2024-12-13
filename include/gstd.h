#ifndef _gstd_h
#define _gstd_h

#include <glog.h>

extern struct glog__logger* gstd__logger;

void gstd__init(struct glog__logger* logger);

#endif
