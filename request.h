#ifndef REQUEST_H
#define REQUEST_H
#include "config.h"
#include "response.h"

char* send_post_request(const char*, int, const char*);
char* read_line(int);
int read_buf(int,char*,int);
void parse_request(int,const char*);
char* decode_request(int);
rpc_request_t* parse_rpc_request(XMLNode* );
rpc_param_t parse_rpc_param(XMLNode*);
void dump_rpc_request(rpc_request_t*);
void dump_rpc_param(rpc_param_t, const char*);
#endif