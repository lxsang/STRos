#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H
#include "request.h"

void subscribe_to(const char*,const char*,void (*)(item));
#endif