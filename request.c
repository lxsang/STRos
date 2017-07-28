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
#include "request.h"

void parse_request(int client,const char* uri,void (*handler)(int,void*))
{
	XMLDoc doc;
	char* request = decode_rpc_data(client);
	if(request == NULL)
	{
		rpc_bad_request(client);
		return;
	}
	//printf("Received:\n%s\n",request);
	// parse the xml
	XMLDoc_init(&doc);
	if(!XMLDoc_parse_buffer_DOM(request,"xmlprc_request",&doc))
	{
		LOG("%s","Unable to parser XML request");
		rpc_bad_request(client);
		return;
	}
	XMLNode* node;
	// root node
	node = doc.nodes[doc.i_root];
	if(strcmp(node->tag,MCALL_TAG) != 0)
	{
		LOG("%s","Invalid XML request");
		rpc_bad_request(client);
		return;
	}
	rpc_request_t* m = parse_rpc_method(node);
	if(!m)
	//	dump_rpc_method(m);
	//else
	{
		LOG("%s","Fail to parse the method\n");
		rpc_bad_request(client);
		return;
	}
	XMLDoc_free(&doc);
	if(handler == NULL)
	{
		LOG("%s","Send a dummy response\n");
		rpc_dummy_response(client);
		free_request(m);
		return;
	}
	
	handler(client,m);
	free_request(m);
}
void dump_rpc_method(rpc_request_t* m)
{
	printf("Method name %s\n", m->method);
	printf("Params:\n");
	printf("[%s]\n",as_string(m->params));
}

rpc_request_t* parse_rpc_method(XMLNode* mnode)
{
	XMLNode* node;
	// find method name
	if(mnode->n_children != 2) return NULL;
	if(strcmp(mnode->children[0]->tag,MNAME_TAG) != 0) return NULL;
	rpc_request_t* req = (rpc_request_t*) malloc(sizeof(rpc_request_t));
	node = mnode->children[0];
	req->method = strdup(node->text);
	node = mnode->children[1];
	if(strcmp(node->tag,"params") != 0){ 
		LOG("The <params> tag not found:%s\n",node->tag);
		free(req);
		return NULL;
	}
	// collect the params
	//node = node->children[0];
	req->n_params = node->n_children;
	req->params = list_init();
	for(size_t i = 0; i < node->n_children; ++i)
	{
		if(strcmp(node->children[i]->tag,"param") != 0) {free(req);return NULL;}
		parse_rpc_value(&req->params,node->children[i]);
	}
	return req;
}
int request_socket(const char* ip, int port)
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	//char buf[MAX_BUFF];
	char* request;
	// time out setting
	struct timeval timeout;      
	timeout.tv_sec = CONN_TIME_OUT_S;
	timeout.tv_usec = 0;
	if ( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )// PF_INET instead of AF
	{
		perror("Socket");
		return -1;
	}
	if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
	        perror("setsockopt failed\n");

    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        perror("setsockopt failed\n");
	
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if ( inet_aton(ip, &dest.sin_addr.s_addr) == 0 )
    {
		perror(ip);
		close(sockfd);
		return -1;
    }
	if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
	{
		close(sockfd);
		perror("Connect");
		return -1;
	}
	return sockfd;
}
rpc_response_t* send_post_request(const char* ip, int port, const char* data)
{
	int sockfd, bytes_read;
	rpc_response_t* rdata = NULL;
	char* request;
	if((sockfd = request_socket(ip, port)) != -1)
	{
    	request = __s(REQUEST_PATTERN, strlen(data), data);
		send(sockfd,request, strlen(request),0);
		rdata = parse_response(sockfd);
		close(sockfd);
		return rdata;
	}
	return NULL;
}
void free_request(rpc_request_t* r)
{
	if(r->params != NULL)
		list_free(&r->params);
	free(r);
}
char* gen_rpc_call(rpc_request_t* rq)
{
	char* xmldoc = __s(XML_METHOD, rq->method);
	xmldoc = __s("%s%s",xmldoc,list_to_xml_params(rq->params));
	xmldoc = __s(XML_RPC_CALL,xmldoc);
	return __s("%s%s",XML_VER,xmldoc);
}
