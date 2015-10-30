#ifndef STROS_H
#define STROS_H
#include "config.h"
#include "3rd/ini/ini.h"
#include "dictionary.h"
#include "request.h"
#include "response.h"

#define DEFAULT_MASTER "127.0.0.1:11311"
#define subscriber topic_t;
#define publisher  topic_t;

typedef struct{
	char* id;
	char* api;
	dictionary subscribers;
	dictionary publishers;
	char* master;
} ros_node_t;

typedef struct{
	char* topic;
	char* type;
	char* api;
	int event;
	void* data;
} topic_t;

/**
* The init_node method will start a xmlprc
* server dedicated for this node, we used separated
* xmlprc server for each node so that a crash on
* a node will not cause the others nodes to stop 
* working
*/
void stros_init_node(ros_node_t*, const char*, const char*);
/*
* Deploy all subscribes and publishers :
*	- registe topics (publishers)
*	- accquire information for subscribles
*	- run publishers in a separe thread
*	- run sbscribers in a separe thread
* use mutex to prevent data race condition
*/
void stros_node_deploy(ros_node_t);
/**
* Unregiste all active publishers and subscribers
* stop all related threads
* stop the node
**/
void stros_stop(int); 
/**
* Is the ros client running
**/
int stros_ok();

void error_die(const char *);
void load_config(ros_node_t*,const char*);
void accept_request(int);
int xmlprc_startup(unsigned *);

//static char* xmlprc_config;
static int ros_ok;
// mutex lock
pthread_mutex_t node_mux;
#endif