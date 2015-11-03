/*
The MIT License (MIT)

Copyright (c) 2015 LE Xuan Sang xsang.le@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef STROS_H
#define STROS_H
#include "config.h"
#include "3rd/ini/ini.h"
#include "api.h"

#define DEFAULT_MASTER_URI "127.0.0.1"
#define DEFAULT_MASTER_PORT 11311
#define unregister_subscriber(s) (unregister(s,"unregisterSubscriber"))
#define unregister_publisher(s) (unregister(s,"unregisterPublisher"))

/**
* The init_node method will start a xmlprc
* server dedicated for this node, we used separated
* xmlprc server for each node so that a crash on
* a node will not cause the others nodes to stop 
* working
*/
ros_node_t* stros_init_node(const char*, const char*);
/*
* Deploy all subscribes and publishers :
*	- registe topics (publishers)
*	- accquire information for subscribles
*	- run publishers in a separe thread
*	- run sbscribers in a separe thread
* use mutex to prevent data race condition
*/
void stros_node_deploy(ros_node_t*);
/**
* Unregiste all active publishers and subscribers
* stop all related threads
* stop the node
**/
void stros_stop(int); 
/**
* Is the ros client running
**/

void subscribe_to(ros_node_t*,const char* topic,const char* type,void (*handler)(void*));
void error_die(const char *);
void load_config(ros_node_t*,const char*);
void accept_request(int);
int xmlprc_startup(unsigned *);
void xmlprc_run(unsigned* );
void dump_node_info(ros_node_t*);
void stros_deploy_subscribers(ros_node_t*);
void subscriber_listener(void*);
void stros_deploy_publishers(ros_node_t*);
void spin();
publisher* create_publisher(ros_node_t* , const char* , const char* );
void publish(publisher*,void*);
void publisher_listener(void*);
publisher* create_publisher(ros_node_t*, const char*, const char*);
void unregister(topic_t*, const char* );
//static char* xmlprc_config;

// mutex lock
#endif