#ifndef REQUEST_H
#define REQUEST_H
#include "config.h"
#include "response.h"

typedef struct{
	char* method;
	int n_params;
	rpc_value_t params;
} rpc_request_t;

rpc_response_t* send_post_request(const char*, int, const char*);
void parse_request(int,const char*);
rpc_request_t* parse_rpc_method(XMLNode* );
void dump_rpc_method(rpc_request_t*);
void free_request(rpc_request_t*);
char* gen_rpc_call(rpc_request_t*);
#endif