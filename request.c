#include "request.h"
/**
 * read the request as a string line format
 * @param  sock socket
 * @return      a request string
 */
char* read_line(int sock)
{
	char buf[MAX_BUFF];
	read_buf(sock,buf,sizeof(buf));
	return strdup(buf);
}
/**
 * Read the socket request in to a buffer or size
 * The data is read until the buffer is full or
 * there are a carrier return character
 * @param  sock socket
 * @param  buf  buffer
 * @param  size size of buffer
 * @return      number of bytes read
 */
int read_buf(int sock, char*buf,int size)
{
	int i = 0;
	char c = '\0';
	int n;
	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0);
		if (n > 0)
		{
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';
	return i;
}
void parse_request(int client,const char* uri)
{
	XMLDoc doc;
	char* request = decode_request(client);
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
	rpc_request_t* m = parse_rpc_request(node);
	if(m)
		dump_rpc_request(m);
	else
		LOG("%s","Fail to parse the method\n");
	XMLDoc_free(&doc);
	dummy_response(client);
	
}
void dump_rpc_request(rpc_request_t* m)
{
	printf("Method name %s\n", m->method);
	printf("Params:\n");
	for(size_t i = 0; i < m->n_params; ++i)
	{
		dump_rpc_param(m->params[i],"\t");
	}
}
void dump_rpc_param(rpc_param_t p, const char* prefix)
{
	switch(p.type)
	{
		case RPC_TYPE_BASE64:
		printf("%sBase64:%s\n", prefix,p.value.b64);
		break;
		
		case RPC_TYPE_BOOL:
		printf("%sBool:%d\n", prefix,p.value.b);
		break;
		
		case RPC_TYPE_DOUBLE:
		printf("%sDouble:%lf\n", prefix,p.value.d);
		break;
		
		case RPC_TYPE_DATE:
		printf("%sDate:%s\n", prefix,p.value.date);
		break;
		
		case RPC_TYPE_INT:
		case RPC_TYPE_I4:
		printf("%sInt:%d\n", prefix,p.value.i);
		break;
		
		case RPC_TYPE_STRING:
		printf("%string:%s\n", prefix,p.value.s);
		break;
		
		default:
		LOG("Nil value detected\n");
		break;
	}
}
rpc_request_t* parse_rpc_request(XMLNode* mnode)
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
	for(size_t i = 0; i < node->n_children; ++i)
	{
		if(strcmp(node->children[i]->tag,"param") != 0) {free(req);return NULL;}
		req->params[i] = parse_rpc_param(node->children[i]);
	}
	return req;
}
rpc_param_t parse_rpc_param(XMLNode* pnode)
{
	XMLNode* node;
	rpc_param_t param={.type=RPC_TYPE_NIL};
	if (pnode->n_children <= 0 || strcmp(pnode->children[0]->tag,"value") != 0) return param;
	node = pnode->children[0];
	if(node->n_children <= 0) return param;
	node = node->children[0];
	switch(hash(node->tag))
	{
		case RPC_TYPE_BASE64:
		param.type = RPC_TYPE_BASE64;
		param.value.b64 = strdup(node->text);
		break;
		
		case RPC_TYPE_BOOL:
		param.type = RPC_TYPE_BOOL;
		param.value.b = atoi(node->text);
		break;
		
		case RPC_TYPE_DOUBLE:
		param.type = RPC_TYPE_DOUBLE;
		sscanf(node->text, "%lf", &param.value.d);
		break;
		
		case RPC_TYPE_DATE:
		param.type = RPC_TYPE_DATE;
		param.value.date = strdup(node->text);
		break;
		
		case RPC_TYPE_INT:
		case RPC_TYPE_I4:
		param.type = RPC_TYPE_INT;
		param.value.i = atoi(node->text);
		break;
		
		case RPC_TYPE_STRING:
		param.type = RPC_TYPE_STRING;
		param.value.s = strdup(node->text);
		break;
		
		default:
		LOG("This type is not support yet : %s\n",node->tag);
		break;
	}
	return param;
}
char* send_post_request(const char* ip, int port, const char* data)
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	char* rdata = "";
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
	do
	{
		 bzero(buf, sizeof(buf));
		 bytes_read = recv(sockfd, buf, sizeof(buf), 0);
		 if(bytes_read >0)
			 rdata = __s("%s%s",rdata,buf);
	} while(bytes_read>0);
	close(sockfd);
	return rdata;
}
/**
 * Decode the HTTP request
 * if it is a POST, check the content type of the request
 * 		- if it is a POST request with URL encoded : decode the url encode
 * @param  client socket client
 * @return        a xml string
 */
char* decode_request(int client)
{
	char* request = NULL;
	char* line;
	char * token;
	char* ctype = NULL;
	int clen = -1;
	line = read_line(client);
	while ((strlen(line) > 0) && strcmp("\r\n",line))
	{
		token = strsep(&line,":");
		trim(token,' ');
		if(token != NULL &&strcasecmp(token,"Content-Type") == 0)
		{
			ctype = strsep(&line,":");
			trim(ctype,' ');
			trim(ctype,'\n');
			trim(ctype,'\r');
		} else if(token != NULL &&strcasecmp(token,"Content-Length") == 0)
		{
			token = strsep(&line,":");
			trim(token,' ');
			clen = atoi(token);
		}
		line = read_line(client);
	}
	free(line);
	if(ctype == NULL || clen == -1)
	{
		LOG("Bad request\n");
		return NULL;
	}
	LOG("Content-type: %s",ctype);
	// decide what to do with the data
	//get xml string
	request = (char*) malloc(clen*sizeof(char));
	for(size_t i = 0; i < clen; ++i)
		recv(client,(request+i),1,0);
	return request;
}