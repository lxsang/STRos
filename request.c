#include "request.h"

void parse_request(int client,const char* uri)
{
	XMLDoc doc;
	char* request = decode_rpc_data(client);
	if(request == NULL)
	{
		bad_request(client);
		return;
	}
	printf("Received:\n%s\n",request);
	// parse the xml
	XMLDoc_init(&doc);
	if(!XMLDoc_parse_buffer_DOM(request,"xmlprc_request",&doc))
	{
		LOG("%s","Unable to parser XML request");
		bad_request(client);
		return;
	}
	XMLNode* node;
	// root node
	node = doc.nodes[doc.i_root];
	if(strcmp(node->tag,MCALL_TAG) != 0)
	{
		LOG("%s","Invalid XML request");
		bad_request(client);
		return;
	}
	rpc_request_t* m = parse_rpc_method(node);
	if(m)
	{
		dump_rpc_method(m);
		free_request(m);
	}
	else
		LOG("%s","Fail to parse the method\n");
	XMLDoc_free(&doc);
	dummy_response(client);
	
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

rpc_response_t* send_post_request(const char* ip, int port, const char* data)
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	rpc_response_t* rdata = NULL;
	char buf[MAX_BUFF];
	char* request;
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		return NULL;
	}
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if ( inet_aton(ip, &dest.sin_addr.s_addr) == 0 )
    {
		perror(ip);
		return NULL;
    }
	if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
	{
		perror("Connect");
		return NULL;
	}
    request = __s(REQUEST_PATTERN, strlen(data), data);
	send(sockfd,request, strlen(request),0);
	rdata = parse_response(sockfd);
	// do
// 	{
// 		 bzero(buf, sizeof(buf));
// 		 bytes_read = recv(sockfd, buf, sizeof(buf), 0);
// 		 if(bytes_read >0)
// 			 rdata = __s("%s%s",rdata,buf);
// 	} while(bytes_read>0);
	close(sockfd);
	return rdata;
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
