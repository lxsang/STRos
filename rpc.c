#include "rpc.h"
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

void parse_rpc_value(rpc_value_t* l,XMLNode* pnode)
{
	XMLNode* node;
	if(strcmp(pnode->tag,"value") == 0)
	{
		node = pnode;
	}
	else
	{
		if (pnode->n_children <= 0 || strcmp(pnode->children[0]->tag,"value") != 0) return;
		node = pnode->children[0];
	}
	if(node->n_children <= 0) return;
	node = node->children[0];
	switch(hash(node->tag))
	{
		case RPC_TYPE_BASE64:
		list_put_b64(l,node->text);
		break;
		
		case RPC_TYPE_BOOL:
		list_put_b(l,atoi(node->text));
		break;
		
		case RPC_TYPE_DOUBLE:
		list_put_special(l,node->text);
		break;
		
		case RPC_TYPE_DATE:
		list_put_date(l,node->text);
		break;
		
		case RPC_TYPE_INT:
		case RPC_TYPE_I4:
		list_put_i(l,atoi(node->text));
		break;
		
		case RPC_TYPE_STRING:
		list_put_s(l,node->text);
		break;
		
		case RPC_TYPE_ARRAY:
		if(node->n_children != 1 || strcmp(node->children[0]->tag,"data") != 0) return;
		list arr = list_init();
		for(size_t i = 0; i < node->children[0]->n_children; ++i)
		{
			parse_rpc_value(&arr,node->children[0]->children[i]);
		}
		list_put_array(l,arr);
		break;
		
		default:
		LOG("This type is not support yet : %s\n",node->tag);
		break;
	}
}
/**
 * Decode the HTTP request
 * if it is a POST, check the content type of the request
 * 		- if it is a POST request with URL encoded : decode the url encode
 * @param  client socket client
 * @return        a xml string
 */
char* decode_rpc_data(int client)
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
		LOG("Bad data\n");
		return NULL;
	}
	LOG("Content-length: %d\n",clen);
	// decide what to do with the data
	//get xml string
	request = (char*) malloc(clen*sizeof(char));
	for(size_t i = 0; i < clen; ++i)
		recv(client,(request+i),1,0);
	return request;
}
char* list_to_xml_params(list l)
{
	char* pnode,*dnode;
	item np;
	pnode = "";
	for(np = l;np != NULL;np=np->next)
	{
		dnode = __s(XML_PAR,item_to_xml(np));
		pnode = __s("%s%s",pnode,dnode);
	}
	return __s(XML_PARS,pnode);
}
char* item_to_xml(item it)
{
	char *dnode;
	item np;
	switch(it->type)
	{
		case RPC_TYPE_BASE64:
		dnode = __s(XML_B64,it->value.b64);
		break;
	
		case RPC_TYPE_BOOL:
		dnode = __s(XML_BOOL,it->value.b);
		break;
	
		case RPC_TYPE_DOUBLE:
		dnode = __s(XML_DBLE,it->value.d);
		break;
	
		case RPC_TYPE_DATE:
		dnode = __s(XML_DATE,it->value.date);
		break;
	
		case RPC_TYPE_INT:
		case RPC_TYPE_I4:
		dnode = __s(XML_INT,it->value.i);
		break;
	
		case RPC_TYPE_STRING:
		dnode = __s(XML_STR,it->value.s);
		break;
		
		case RPC_TYPE_ARRAY:/*a bit more complicate*/
		dnode = "";
		for(np = it->value.array;np!=NULL;np=np->next)
			dnode = __s("%s%s", dnode,item_to_xml(np));
		dnode = __s(XML_ARR,dnode);
		break;
		
		
		default:
		dnode = XML_NIL;
		break;
	}
	return __s(XML_VALUE,dnode);
}
char* get_ip_address()
{
	struct ifaddrs* addrs;
	getifaddrs(&addrs);
	struct ifaddrs* tmp = addrs;
	char* ip;
	while (tmp) 
	{
	    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
	    {
			struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
	        ip = inet_ntoa(pAddr->sin_addr);
			if(strcmp(ip,"127.0.0.1") != 0) 
				return ip;
	    }
	    tmp = tmp->ifa_next;
	}
	freeifaddrs(addrs);
	return "127.0.0.1";
}