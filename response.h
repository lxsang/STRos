#ifndef RESPONSE_H
#define RESPONSE_H
#include "config.h"
#include "rpc.h"

#define RESP_OK		1
#define RESP_FAULT	0

typedef struct{
	int code;
	int n_value;
	rpc_value_t data;
} rpc_response_t;

void bad_request(int);
void dummy_response(int);
rpc_response_t* parse_response(int);
void parse_fault_response(rpc_value_t*,XMLNode*);
void dump_rpc_response(rpc_response_t* );
#endif