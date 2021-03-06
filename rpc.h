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
#ifndef RPC_H
#define RPC_H
#include "list.h"
#include "3rd/xml/sxmlc.h"
#include "config.h"
#define REQUEST_PATTERN  "POST /RPC2 HTTP/1.0\r\nHost: stROS\r\nUser-Agent: stROS\r\nContent-Type: text/xml\r\nContent-Length: %d\r\n\r\n%s"
#define RESPONSE_PATTERN "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %d\r\nContent-Type: text/xml\r\nServer: %s\r\n%s"
#define MCALL_TAG		"methodCall"
#define MNAME_TAG 		"methodName"
#define MRESP_TAG 		"methodResponse"

#define XML_VER 		"<?xml version=\"1.0\"?>\r\n"
#define XML_RPC_CALL	"<methodCall>\r\n%s\r\n</methodCall>"
#define XML_METHOD		"<methodName>%s</methodName>\r\n"
#define XML_PARS		"<params>\r\n%s\r\n</params>\r\n"
#define XML_PAR			"<param>\r\n%s\r\n</param>\r\n"
#define XML_VALUE		"<value>%s</value>\r\n"
#define XML_INT			"<int>%d</int>"
#define XML_BOOL		"<boolean>%d</boolean>"
#define XML_B64			"<base64>%s</base64>"
#define XML_DATE		"<dateTime.iso8601>%s</dateTime.iso8601>"
#define XML_DBLE		"<double>%lf</double>"
#define XML_STR			"<string>%s</string>"
#define XML_ARR			"<array>\r\n<data>\r\n%s</data>\r\n</array>\r\n"
#define XML_NIL			"<nil/>\r\n"
#define XML_RPC_RESP	"<methodResponse>\r\n%s\r\n</methodResponse>"


#define rpc_value_t item
/*
typedef struct {
	unsigned type;
	union{
		char *s;
		int b;
		double d;
		char* date;
		int i;
		char* b64;
		list array;
	} value;
} rpc_param_t;*/



void parse_rpc_value(rpc_value_t*,XMLNode*);
char* rpc_read_line(int);
int rpc_read_buf(int,char*,int);
char* decode_rpc_data(int);
char* item_to_xml(item);
char* list_to_xml_params(list);
char* get_ip_address();
#endif