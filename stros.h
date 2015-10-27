#ifndef STROS_H
#define STROS_H
#include "config.h"
#include "3rd/ini/ini.h"
#include "request.h"
#include "response.h"

void error_die(const char *);
void init_file_system();
void load_config(const char*);
void accept_request(int);
int startup(unsigned *);
#endif