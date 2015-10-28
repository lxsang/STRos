#ifndef CONFIG_H
#define CONFIG_H
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <errno.h>
#include <ifaddrs.h>
#include "utils.h"

#define CONFIG "config.ini"
#define MAX_BUFF 1024
#define IS_SPACE(x) isspace((int)(x))
#define SERVER_STRING "Server: stros-xmlprc\r\n"
#define RPC_URI "/RPC2"


typedef struct  { 
	int port;
    char *master_url; 
    int master_port;
}config_t;

config_t server_config;

#endif