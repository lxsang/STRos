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
#include "utils.h"
#include "3rd/xml/sxmlc.h"

#define CONFIG "config.ini"
#define MAX_BUFF 1024
#define IS_SPACE(x) isspace((int)(x))
#define SERVER_STRING "Server: stros-xmlprc\r\n"
#define RPC_URI "/RPC2"
#define REQUEST_PATTERN  "POST /RPC2 HTTP/1.0\r\nHost: stROS\r\nUser-Agent: stROS\r\nContent-Type: text/xml\r\nContent-Length: %d\r\n\r\n%s"
#define XML_VER "<?xml version="1.0"?>"
#define MCALL_TAG		"methodCall"
#define MNAME_TAG 		"methodName"
#define MRESP_TAG 		"methodResponse"
#define RPC_TYPE_ARRAY	601//hash("array")
#define RPC_TYPE_BASE64	335//hash("base64")
#define RPC_TYPE_BOOL	40//hash("boolean")
#define RPC_TYPE_DOUBLE	977//hash("double")
#define RPC_TYPE_DATE	49//hash("dateTime.iso8601")
#define RPC_TYPE_INT	1007//hash("int")
#define RPC_TYPE_I4		235//hash("i4")
#define RPC_TYPE_STRING	17//hash("string")
#define RPC_TYPE_NIL	529//hash("nil")
#define RPC_MAX_PARAM	10

/*the struct and array type is not supported at the moment*/
typedef struct {
	unsigned type;
	union{
		char *s;
		int b;
		double d;
		char* date;
		int i;
		char* b64;
	} value;
} rpc_param_t;

typedef struct{
	char* method;
	int n_params;
	rpc_param_t params[RPC_MAX_PARAM];
} rpc_request_t;


typedef struct  { 
	int port;
    char *master_url; 
    int master_port;
}config_t;

config_t server_config;

#endif