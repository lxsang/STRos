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
#include "api.h"

void stros_toggle(int value)
{
	pthread_mutex_lock (&node_mux);
	ros_ok = value;
	pthread_mutex_unlock (&node_mux);
}
int stros_ok()
{
	return ros_ok;
}

void xml_rpc_api_handler(int client, void* data)
{
	int mt_hash;
	rpc_request_t* request = (rpc_request_t*)data;
	if(request->method)
	{
		mt_hash = hash(request->method, HASHSIZE);
		switch(mt_hash)
		{
			case RQ_PUBLISHER_UPDATE:
			publisherUpdate(request->params, request->n_params, client);
			break;
			
			case RQ_REQUEST_TOPIC:
			requestTopic(request->params, request->n_params, client);
			break;
			
			default:
			LOG("The method:%s is unimplemented\n",request->method);
			break;
		}
	}
	else
	{
		rpc_bad_request(client);
	}
}
void publish_tcp_data(void* data)
{
	tcp_pub_t* conn = (tcp_pub_t*)data;
	int size,bytes_io;
	char buf[MAX_BUFF];
	int client = conn->client;
	// get the header size
	bytes_io = recv(client, &size, sizeof(size), 0);
	if(bytes_io<0) return;
	// get the header
	bytes_io = recv(client, buf, size, 0);
	if(bytes_io<=0 || !verify_ros_header(conn->pub,buf, size)) return;
	memset(buf ,0,MAX_BUFF);
	size = gen_tcp_header_for(conn->pub,buf);
	// send then header first
	bytes_io = send(client, buf, size, 0);
	
	if(bytes_io<0) return;
	do {
		// send a loop data
		bytes_io = tcp_send_data(conn->pub,client);
	}while(stros_ok() && conn->pub->status && bytes_io != -1);
	//LOG("Send to client %s\n", conn->pub->topic);
	//rpc_dummy_response(conn->client);
}
int tcp_send_data(topic_t* tp, int client)
{
	int thash = hash(tp->type,HASHSIZE);
	if(tp->data == NULL) return -1;
	switch(thash)
	{
		case TYPE_STRING : 
		return tcp_send_string(tp, client);
		break;
		
		default : 
		// do nothing return raw; 
		return -1;
		break; 	
	}
}
int tcp_send_string(topic_t* tp, int client)
{
	int size, bytes_out;
	char* data = (char*) tp->data;
	size = strlen(data)+4;
	bytes_out = send(client,&size,4,0);
	if(bytes_out<0) return -1;
	size = strlen(data);
	bytes_out = send(client,&size,4,0);
	if(bytes_out<0) return -1;
	return send(client,data,size,0);
}
void tcp_ros_request(const char* ip, int port, subscriber* sub)
{
	int sockfd, bytes_read, dtype;
	char buf[MAX_BUFF];
	int size;
	size = gen_tcp_header_for(sub, buf);
	sockfd = request_socket(ip, port);
	void* data;
	if(sockfd == -1) return;
	// send the header
	send(sockfd,buf, size,0);
    // read the data
	//first read the header size
	bytes_read = recv(sockfd, &size, sizeof(size), 0);
	if(bytes_read <= 0)
	{
		LOG("%s","Cannot read the header size\n");
		close(sockfd);
		return;
	}
	// read the header and check it again subscriber
	bytes_read = recv(sockfd,buf, size,0);
	if(bytes_read <= 0 || !(dtype = verify_ros_header(sub,buf, size)))
	{
		LOG("%s","Cannot read the header\n");
		close(sockfd);
		return;
	}
	do
	{
		memset(buf ,0,MAX_BUFF);
		//now get the size of data
		recv(sockfd, &size, sizeof(size), 0);
		bytes_read = recv(sockfd,buf, size,0);
		if(bytes_read<0)
		{
			LOG("%s", "Cannot get data \n");
			close(sockfd);
			return;
		}
		// now get the right data format and call the callback
		data = ros_data(buf, dtype);
		sub->handler(data, sub);
	} while(bytes_read > 0 && sub->status == TOPIC_ACTIVE && stros_ok()); // should have ros_ok
	
	//read all the remain
	//while((bytes_read = recv(sockfd,buf, MAX_BUFF,0)) > 0) ;
	
	close(sockfd);
}
int verify_ros_header(subscriber* sub, char* header, int hsize)
{
	char *msgtype, *md5sum,*topic,*ptr,*key,*val;
	char field[MAX_BUFF];
	int size;
	ptr = header;
	msgtype = NULL;
	md5sum = NULL;
	topic = NULL;
	list l;
	while((!msgtype || !md5sum || !topic) && ptr < header + hsize)
	{
		memset(field ,0,MAX_BUFF);
		// read field size
		size = (int)(*ptr);
		ptr += 4;
		memcpy(field, ptr ,size);
		ptr += size;
		//LOG("Found field:%s\n",field);
		l = split(field,"=");
		if(list_size(l) != 2) {list_free(&l); continue;}
		key = list_at(l,0)->value.s;
		val = list_at(l,1)->value.s;
		if(strcmp(key,"topic")== 0)
			topic = val;
		else if(strcmp(key,"md5sum")== 0)
			md5sum = val;
		else if(strcmp(key,"type") ==0)
			msgtype = val;
		list_free(&l);
	}
	// check for type, md5 and topic
	if(!msgtype || !md5sum || !topic) 
	{
		LOG("%s","Some fields is missing\n");
		return 0;
	}
	if(strcmp(topic, sub->topic) != 0)
	{
		LOG("Wrong topic, expect [%s] but receive [%s]\n", sub->topic, topic);
		return 0;
	}
	if(strcmp(msgtype, sub->type))
	{
		LOG("Wrong message type, expect [%s] but receive [%s]\n", sub->type, msgtype);
		return 0;
	}
	if(strcmp(md5sum, md5sum_of(msgtype)) != 0)
	{
		LOG("Wrong md5sum signature, receive [%s] for type [%s]\n", md5sum, msgtype);
		return 0;
	}
	return hash(msgtype, HASHSIZE);
}
void* ros_data(char* raw,int type_h)
{
	switch(type_h)
	{
		case TYPE_STRING : 
		return (void*)ros_string(raw);
		break;
		default : 
		return raw; 
		break; 	
	}
}
char* ros_string(char* raw)
{
	int size;
	char *data;
	size = (int)*raw;
	data =(char*) malloc(size*sizeof(char)+1);
	memcpy(data,raw + 4, size);
	data[size] = '\0';
	return data;
}
int gen_tcp_header_for(subscriber* sub, char* buf)
{
	int header_size, size;
	char* tmp,*ptr;
	
	header_size = 0;
	ptr = buf+4;
	
	tmp = __s("message_definition=stros node");
	size = strlen(tmp);
	memcpy(ptr,&size, sizeof(size));
	ptr+= 4;
	memcpy(ptr,tmp, size);
	header_size += size+4;
	ptr += size;
	
	tmp = __s("callerid=%s",sub->callerid);
	size = strlen(tmp);
	memcpy(ptr,&size, sizeof(size));
	ptr+= 4;
	memcpy(ptr,tmp, size);
	header_size += size+4;
	ptr+= size;
	
	tmp = __s("topic=%s",sub->topic);
	size = strlen(tmp);
	memcpy(ptr,&size, sizeof(size));
	ptr+= 4;
	memcpy(ptr,tmp, size);
	header_size += size+4;
	ptr+= size;
	
	tmp = __s("md5sum=%s",md5sum_of(sub->type));
	size = strlen(tmp);
	memcpy(ptr,&size, sizeof(size));
	ptr+= 4;
	memcpy(ptr,tmp, size);
	header_size += size+4;
	ptr+= size;
	
	tmp = __s("type=%s",sub->type);
	size = strlen(tmp);
	memcpy(ptr,&size, sizeof(size));
	ptr+= 4;
	memcpy(ptr,tmp, size);
	header_size += size+4;
	
	memcpy(buf,&header_size, sizeof(header_size));
	return header_size+4;
}
char* md5sum_of(const char* type)
{
	int th = hash(type, HASHSIZE);
	switch(th)
	{
		case TYPE_BOOL 				   : return ROS_MD5_BOOL 				; break;
		case TYPE_BYTE 				   : return ROS_MD5_BYTE 				; break;
		case TYPE_BYTEMULTIARRAY 	   : return ROS_MD5_BYTEMULTIARRAY 		; break;
		case TYPE_CHAR 				   : return ROS_MD5_CHAR 				; break;
		case TYPE_COLORRGBA 		   : return ROS_MD5_COLORRGBA 			; break;	
		case TYPE_DURATION 			   : return ROS_MD5_DURATION 			; break;
		case TYPE_EMPTY 			   : return ROS_MD5_EMPTY 				; break;	
		case TYPE_FLOAT32 			   : return ROS_MD5_FLOAT32 			; break;
		case TYPE_FLOAT32MULTIARRAY	   : return ROS_MD5_FLOAT32MULTIARRAY 	; break;
		case TYPE_FLOAT64 			   : return ROS_MD5_FLOAT64 			; break;
		case TYPE_FLOAT64MULTIARRAY    : return ROS_MD5_FLOAT64MULTIARRAY 	; break;	
		case TYPE_HEADER 			   : return ROS_MD5_HEADER 				; break;
		case TYPE_INT16 			   : return ROS_MD5_INT16 				; break;	
		case TYPE_INT16MULTIARRAY 	   : return ROS_MD5_INT16MULTIARRAY 	; break;
		case TYPE_INT32 			   : return ROS_MD5_INT32 				; break;	
		case TYPE_INT32MULTIARRAY 	   : return ROS_MD5_INT32MULTIARRAY 	; break;
		case TYPE_INT64 			   : return ROS_MD5_INT64 				; break;	
		case TYPE_INT64MULTIARRAY 	   : return ROS_MD5_INT64MULTIARRAY 	; break;
		case TYPE_INT8 				   : return ROS_MD5_INT8 				; break;
		case TYPE_INT8MULTIARRAY 	   : return ROS_MD5_INT8MULTIARRAY 		; break;
		case TYPE_MULTIARRAYDIMENSION  : return ROS_MD5_MULTIARRAYDIMENSION ; break;
		case TYPE_MULTIARRAYLAYOUT 	   : return ROS_MD5_MULTIARRAYLAYOUT 	; break;
		case TYPE_STRING 			   : return ROS_MD5_STRING 				; break;
		case TYPE_TIME 				   : return ROS_MD5_TIME 				; break;
		case TYPE_UINT16 			   : return ROS_MD5_UINT16 				; break;
		case TYPE_UINT16MULTIARRAY 	   : return ROS_MD5_UINT16MULTIARRAY 	; break;
		case TYPE_UINT32 			   : return ROS_MD5_UINT32 				; break;
		case TYPE_UINT32MULTIARRAY 	   : return ROS_MD5_UINT32MULTIARRAY 	; break;
		case TYPE_UINT64 			   : return ROS_MD5_UINT64 				; break;
		case TYPE_UINT64MULTIARRAY 	   : return ROS_MD5_UINT64MULTIARRAY 	; break;
		case TYPE_UINT8 			   : return ROS_MD5_UINT8 				; break;	
		case TYPE_UINT8MULTIARRAY 	   : return ROS_MD5_UINT8MULTIARRAY 	; break;
		default						   : return ROS_MD5_STRING 				; break; 	
	}
}
void publisherUpdate(rpc_value_t params, int n, int client)
{
	//LOG("Number params %d\n", n);
	//LOG("parms: %s\n",as_string(params));
	char* topic = list_at(params,1)->value.s;
	char* url;
	list l;
	if(topic == NULL)
	{
		rpc_bad_request(client);
		return;
	}
	subscriber* sub = (subscriber*)dvalue(xml_prc_server.node->subscribers,topic);
	if(sub == NULL)
	{
		rpc_bad_request(client);
		return;
	}
	l = list_at(params,2)->value.array;
	extract_url(sub, l);
	rpc_dummy_response(client);
}
void requestTopic(rpc_value_t params, int n, int client)
{
	if(n != 3){ rpc_bad_request(client);return;}
	char* topic = list_at(params,1)->value.s;
	if(! topic){rpc_bad_request(client);return;}
	publisher* pub = (publisher*) dvalue(xml_prc_server.node->publishers,topic);
	if(!pub)
	{
		LOG("Unknow topic: %s\n", topic);
		rpc_bad_request(client);
		return;
	}
	// send response
	list l = list_init();
	list l1 = list_init();
	list l2 = list_init();
	
	list_put_s(&l2,"TCPROS");
	list_put_s(&l2,pub->uri);
	list_put_i(&l2,pub->port);
	
	list_put_i(&l1,1);
	list_put_s(&l1,__s("ready on %s:%d",pub->uri, pub->port));
	list_put_array(&l1,l2);
	
	list_put_array(&l,l1);
	char* data = gen_rpc_response(l);
	char* xml_resp = __s(RESPONSE_PATTERN, strlen(data), SERVER_STRING,data);
	send(client,xml_resp, strlen(xml_resp),0);	
	list_free(&l);
}
void extract_url(subscriber* sub, list l)
{
	if(!l || list_size(l) == 0) 
	{
		pthread_mutex_lock (&node_mux);
		sub->uri = NULL;
		sub->port = 0;
		sub->status = TOPIC_REFRESH;
		pthread_mutex_unlock(&node_mux);
		return;
	}
	char* url = list_at(l,0)->value.s;
	l = split(url,"/");
	//printf("list size %d\n",list_size(l));
	url = list_at(l,1)->value.s;
	l = split(url,":");
	pthread_mutex_lock (&node_mux);
	sub->uri = list_at(l,0)->value.s;
	sub->port = list_at(l,1)->value.i;
	sub->status = TOPIC_REFRESH;
	pthread_mutex_unlock (&node_mux);
}