#include "response.h"
void bad_request(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>This is a XML-PRC server, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "you must respect this protocol.\r\n");
	send(client, buf, sizeof(buf), 0);
}
void dummy_response(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0  200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/xml\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<result>OK got it</result>\r\n");
	send(client, buf, strlen(buf), 0);
}

rpc_response_t* parse_response(int client)
{
	XMLDoc doc;
	char* resp = decode_rpc_data(client);
	//printf("%s\n", resp);
	if(resp == NULL)
	{
		LOG("%s","Unable to parse the response\n");
		return NULL;
	}
	//printf("Received:\n%s\n",request);
	// parse the xml
	XMLDoc_init(&doc);
	if(!XMLDoc_parse_buffer_DOM(resp,"xmlprc_response",&doc))
	{
		LOG("%s","Unable to parser XML response\n");
		return NULL;
	}
	XMLNode* node;
	// root node
	node = doc.nodes[doc.i_root];
	if(strcmp(node->tag,MRESP_TAG) != 0 || node->n_children != 1)
	{
		LOG("%s","Invalid XML request\n");
		return NULL;
	}
	node = node->children[0];
	rpc_response_t* re = (rpc_response_t*) malloc(sizeof(rpc_response_t));
	if(strcmp(node->tag,"params") == 0)
	{
		re->code = RESP_OK;
		re->n_value = node->n_children;
		re->data = list_init();
		for(size_t i = 0; i < node->n_children; ++i)
		{
			parse_rpc_value(&re->data, node->children[i]);
		}
	}
	else if(strcmp(node->tag,"fault") == 0)
	{
		re->code = RESP_FAULT;
		re->n_value=2;
		if(node->n_children <= 0) return re;
		re->data= list_init();
		parse_fault_response(&re->data,node->children[0]);
	}
	else
	{
		LOG("%s","Invalid XML request\n");
		free(re);
		return NULL;
	}
	// to be continue
	XMLDoc_free(&doc);
	return re;
}
void parse_fault_response(rpc_value_t* l,XMLNode* enode)
{
	XMLNode* node;
	XMLNode* member;
	if(enode->n_children <= 0 ||strcmp(enode->tag,"value")){ LOG("erro find:%s\n",enode->tag); return;}
	node = enode->children[0];
	if(node->n_children <= 0 ||strcmp(node->tag,"struct")) { LOG("erro find:%s\n",node->tag); return;}
	for(size_t i = 0; i < node->n_children; ++i)
	{
		member = node->children[i];
		if(member->n_children != 2) continue;
		member = member->children[1];
		printf("Member %s\n", member->tag);
		parse_rpc_value(l,member);
	}
}
void dump_rpc_response(rpc_response_t* resp)
{
	printf("Response status : %d\n", resp->code);
	printf("Response data:\n\t [%s]\n", as_string(resp->data));
}
void free_response(rpc_response_t* r)
{
	if(r->data != NULL)
		list_free(&r->data);
	free(r);
}