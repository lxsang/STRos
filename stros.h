#ifndef STROS_H
#define STROS_H
#include "config.h"
#include "3rd/ini/ini.h"
#include "request.h"
#include "response.h"


typedef struct{
	char* id;
	char* api;
	char* topic;
	char* topic_type;
} ros_node_t;
ros_node_t ros_node;
void stros_init_node();
void stros_stop_node(); 
void error_die(const char *);
void load_config(const char*);
void accept_request(int);
int startup(unsigned *);
#endif