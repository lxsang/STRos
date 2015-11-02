#include "stros.h"

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

static int config_handler(void* node, const char* section, const char* name,
                   const char* value)
{
    ros_node_t* pconfig = (ros_node_t*)node;

    if (MATCH("MASTER", "uri")) {
        pconfig->master_uri = strdup(value);
    } else if (MATCH("MASTER", "port")) {
        pconfig->master_port = atoi(value);
    }else if (MATCH("NODE", "remap")) {
        pconfig->id = strdup(value);
    }else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void load_config(ros_node_t* node,const char* file)
{
	if (ini_parse(file, config_handler, node) < 0) {
		LOG("Can't load '%s'\n. Used defaut configuration", file);
	}
	else
	{
		LOG("Using node configuration : %s\n", file);
	}
	// if needed
	//init_file_system();
}
void stros_stop(int dummy) {
	LOG("%s","Stopping the node\n");
	stros_toggle(false);
	pthread_exit(NULL);
	pthread_mutex_destroy(&node_mux);
	//TODO unregiste all publisher and subscriber
	//exit(0);
}
void spin()
{
	while(stros_ok());;
}

ros_node_t* stros_init_node(const char* name, const char* conf)
{
	ros_node_t* node = malloc(sizeof(ros_node_t));
	if(name == NULL) 
	{ 
		LOG("%s", "The node name can not be NULL \n");
		exit(1);
	}
	node->id = name;
	node->master_uri = DEFAULT_MASTER_URI;
	node->master_port = DEFAULT_MASTER_PORT;
	node->subscribers = dict();
	node->publishers = dict();
	if(conf != NULL)
		load_config(node,conf);
	node->id = __s("%s_%u",node->id,(unsigned)time(NULL));
	return node;
}
void accept_request(int client)
{
	char buf[MAX_BUFF];
	int numchars;
	char method[255];
	char url[255];
	size_t i, j;
	numchars = read_buf(client, buf, sizeof(buf));
	i = 0; j = 0;
	while (!IS_SPACE(buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';
	/*only support post*/
	if (strcasecmp(method, "POST"))
	{
		LOG("%s","Bad request");
		bad_request(client);
		return;
	}
	i = 0;
	while (IS_SPACE(buf[j]) && (j < sizeof(buf)))
		j++;
	while (!IS_SPACE(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
	{
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	if(strcmp(url,RPC_URI) != 0)
		bad_request(client);
	else
		parse_request(client, url,xml_rpc_api_handler);
	close(client);
}

void stros_node_deploy(ros_node_t* node)
{
	// ignore the broken PIPE error when writing 
	//or reading to/from a closed socked connection
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, stros_stop);
	xml_prc_server.node = node;
	pthread_mutex_init(&node_mux, NULL);
	stros_toggle(true);
	// start the xmlprc
	pthread_t newthread;
	unsigned port = 0;
	if (pthread_create(&newthread , NULL,(void *(*)(void *))xmlprc_run,(void*) &port ) != 0)
	{
		perror("pthread_create");
		stros_stop(0);
	}
	pthread_mutex_lock (&node_mux);
	xml_prc_server.thread_id = newthread;
	pthread_mutex_unlock (&node_mux);
	pthread_detach(newthread) ;
	// so we have an api running
	while(port == 0 && stros_ok()) ;; //wait for the new port comming 
	node->api = __s("http://%s:%d",get_ip_address(),port);
	dump_node_info(node);
	// deploy subscriber
	stros_deploy_subscribers(node);
	// deploy publishers
	stros_deploy_publishers(node);
	LOG("%s","Node deployed\n");
}
void stros_deploy_publishers(ros_node_t* node)
{
	association assoc;
	publisher* pub;
	rpc_request_t* rq = malloc(sizeof(rpc_request_t));
	rpc_response_t* data;
	rq->method = "registerPublisher";
	pthread_t newthread;
	for_each_assoc(assoc, node->publishers)
	{
		// create an request to the master to subscribe to a topic
		pub = (publisher*) (assoc->value);
		rq->params = list_init();
		list_put_s(&rq->params,node->id);
		list_put_s(&rq->params,pub->topic);
		list_put_s(&rq->params,pub->type);
		list_put_s(&rq->params,node->api);
		char* xml = gen_rpc_call(rq);
		data = send_post_request(node->master_uri, node->master_port, xml);
		//printf("DATA : %s\n", as_string(data->data));
		// examine data and call the call back
		if(data->code)
		{
			// create new thread for the publisher
			if (pthread_create(&newthread , NULL,(void *(*)(void *))publisher_listener,(void*) pub ) != 0)
				perror("can not start the topic\n");
				// TODO: we should unregister the publisher here
			else
				pthread_detach(newthread) ;
			LOG("publish %s successful \n",pub->topic);
		}
		else
		{
			LOG("Cannot publish %d \n", pub->topic);
		}
		list_free(&rq->params);
		free_response(data);
	}
}
void publisher_listener(void* data)
{
	publisher* pub = (publisher*) data;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;
	struct timeval timeout;      
	timeout.tv_sec = CONN_TIME_OUT_S;
	timeout.tv_usec = 0;
	// we use select with a timeout
	fd_set active_fd_set;
	// run a socket an wait for connection
	//get the socket
	int port = 0;
	int client_sock= -1;
	int sock = socket_startup(&port);
	if(sock == -1 || port == 0)
	{
		LOG("Cannot run publisher %s\n", pub->topic);
		// TODO we should unpuregister the topic from the master here
		return;
	}
	// modify the publisher
	pthread_mutex_lock (&node_mux);
	pub->uri = get_ip_address();
	pub->port = port;
	pthread_mutex_unlock (&node_mux);
	
	while (stros_ok() && pub->status)
	{
		 FD_ZERO (&active_fd_set);
		 FD_SET (sock, &active_fd_set);
		//read_fd_set = active_fd_set;
		if (select (FD_SETSIZE, &active_fd_set, NULL, NULL, &timeout) < 0)
		{
			perror ("select");
		    continue;
		}
		if (FD_ISSET (sock, &active_fd_set))
		{
			client_sock = accept(sock,(struct sockaddr *)&client_name,&client_name_len);
			if (client_sock == -1)
			{
				perror("Cannot accept client request\n");
				continue;
			}
			setsockopt (client_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
			                sizeof(timeout));
			setsockopt (client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
		                sizeof(timeout));
			tcp_pub_t* conn = malloc(sizeof(tcp_pub_t));
			conn->pub = pub;
			conn->client = client_sock;
			if (pthread_create(&newthread , NULL,(void *(*)(void*)) publish_tcp_data, (void*)conn) != 0)
				perror("pthread_create");
			else
			{
				//reclaim the stack data when thread finish
				pthread_detach(newthread) ;
			}
			 //FD_CLR (i, &active_fd_set);
		}
	}
	
	close(sock);
	
}
void stros_deploy_subscribers(ros_node_t* node)
{
	association assoc;
	subscriber* sub;
	rpc_request_t* rq = malloc(sizeof(rpc_request_t));
	rpc_response_t* data;
	rq->method = "registerSubscriber";
	pthread_t newthread;
	for_each_assoc(assoc, node->subscribers)
	{
		// create an request to the master to subscribe to a topic
		sub = (subscriber*) (assoc->value);
		rq->params = list_init();
		list_put_s(&rq->params,node->id);
		list_put_s(&rq->params,sub->topic);
		list_put_s(&rq->params,sub->type);
		list_put_s(&rq->params,node->api);
		char* xml = gen_rpc_call(rq);
		data = send_post_request(node->master_uri, node->master_port, xml);
		//printf("DATA : %s\n", list_at(data->data,1)->value.s);
		// examine data and call the call back
		if(data->code)
		{
			extract_url(sub, list_at(data->data->value.array,2)->value.array);
			//sub->status = TOPIC_REFRESH;
			// create new thread for the subscribe
			if (pthread_create(&newthread , NULL,(void *(*)(void *))subscriber_listener,(void*) sub ) != 0)
				perror("can not start subscriber");
			//TODO we should unregister the subscriber here
			else
				pthread_detach(newthread) ;
			LOG("Subscribe to %s successful \n",sub->topic);
		}
		else
		{
			LOG("Cannot subscribe to %d \n", sub->topic);
		}
		list_free(&rq->params);
		free_response(data);
	}
}
void subscriber_listener(void* data)
{
	subscriber * sub = (subscriber*) data;
	rpc_request_t* rq = malloc(sizeof(rpc_request_t));
	rq->params = list_init();
	rpc_response_t* resp;
	rq->method = "requestTopic";
	list l1 = list_init();
	list l2 = list_init();
	list_put_s(&rq->params,sub->callerid);
	list_put_s(&rq->params,sub->topic);
	list_put_s(&l2,"TCPROS");
	list_put_array(&l1,l2);
	list_put_array(&rq->params,l1);
	char* xml = gen_rpc_call(rq);
	free_request(rq);
	char*ip = NULL;
	int port = 0;
	while(stros_ok() && sub->status)
	{
		//LOG("Comminicating with %s\n",sub->topic);
		if(sub->uri)
		{
			// require tcp socket for the topic
			if(sub->status == TOPIC_REFRESH)
			{
				resp = send_post_request(sub->uri, sub->port, xml);
				if(resp && resp->code)
				{
					LOG("array of%d elements : %s\n", resp->n_value, as_string(resp->data));
					//get the ip and the port
					l1 = list_at(resp->data->value.array,2)->value.array;
					if(l1 && list_size(l1) == 3)
					{
						pthread_mutex_lock (&node_mux);
						sub->status = TOPIC_ACTIVE;
						pthread_mutex_unlock (&node_mux);
						ip = list_at(l1,1)->value.s;
						port = list_at(l1,2)->value.i;
					}
				}
			}
			if(ip && port > 0)
			{
				tcp_ros_request(ip,port,sub);
			}
		}
	}
}
void publish(publisher* pub, void* msg)
{
	pthread_mutex_lock (&node_mux);
	pub->data = msg;
	pthread_mutex_unlock (&node_mux);
}
void dump_node_info(ros_node_t* node)
{
	LOG("node name:%s\n",node->id);
	LOG("node api:%s\n", node->api);
	LOG("node master:%s:%d\n",node->master_uri, node->master_port);
}
void xmlprc_run(unsigned* port)
{
	int client_sock = -1;
	int server_sock = -1;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;
	// time out setting
	struct timeval timeout;      
	timeout.tv_sec = CONN_TIME_OUT_S;
	timeout.tv_usec = 0;
	// we use select with a timeout, to avoid
	// the thread to be bloked while waiting for
	// a new connection
	// a problem is that, if a client is crash while
	// the server is accepting the socket, we enter another
	// block situation, so we need a way out for that
	fd_set active_fd_set;
  	/* Initialize the set of active sockets. */
   	
	server_sock = socket_startup(port);
	if(server_sock == -1) 
	{
		LOG("Cannot start xmlprc server. Exit \n");
		exit(1);
	}
	LOG("xmlprc running on port %d\n", *port);
	pthread_mutex_lock (&node_mux);
	xml_prc_server.server_sock = server_sock;
	xml_prc_server.pid = getpid();
	pthread_mutex_unlock (&node_mux);
	while (stros_ok())
	{
		 FD_ZERO (&active_fd_set);
		 FD_SET (server_sock, &active_fd_set);
		//read_fd_set = active_fd_set;
		if (select (FD_SETSIZE, &active_fd_set, NULL, NULL, &timeout) < 0)
		{
			perror ("select");
		    continue;
		}
		if (FD_ISSET (server_sock, &active_fd_set))
		{
			client_sock = accept(server_sock,(struct sockaddr *)&client_name,&client_name_len);
			if (client_sock == -1)
			{
				perror("Cannot accept client request\n");
				continue;
			}
			if (setsockopt (client_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
			                sizeof(timeout)) < 0)
			        perror("setsockopt failed\n");

		    if (setsockopt (client_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
		                sizeof(timeout)) < 0)
		        perror("setsockopt failed\n");
			/* accept_request(client_sock); */
			LOG("%s", "Client accepted\n");
			if (pthread_create(&newthread , NULL,(void *(*)(void *))accept_request, (void *)client_sock) != 0)
				perror("pthread_create");
			else
			{
				//reclaim the stack data when thread finish
				pthread_detach(newthread) ;
			}
			 //FD_CLR (i, &active_fd_set);
		}
	}
	LOG("Stop xmlprc server on port %d\n",*port);
	close(server_sock);
	pthread_exit(NULL);
}
int socket_startup(unsigned *port)
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpd == -1)
		return -1;
		//error_die("socket");
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		return -1;
		//error_die("bind");
	if (*port == 0)  /* if dynamically allocating a port */
	{
		socklen_t namelen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
			return -1;
			//error_die("getsockname");
		*port = ntohs(name.sin_port);
	}
	if (listen(httpd, 5) < 0)
		return -1;
		//error_die("listen");
	return(httpd);
}
void subscribe_to(ros_node_t* node, const char* topic,const char* type,void (*handler)(void*))
{
	subscriber* sub = malloc(sizeof(subscriber));
	sub->callerid = node->id;
	sub->topic = topic;
	sub->type = type;
	sub->status = TOPIC_DOWN;
	sub->uri = NULL;
	sub->port = 0;
	sub->data = NULL;
	sub->handler = handler;
	dput(node->subscribers,topic,(void*) sub);
	
}
publisher* create_publisher(ros_node_t* node, const char* topic, const char* type)
{
	publisher* pub = malloc(sizeof(publisher));
	pub->callerid = node->id;
	pub->topic = topic;
	pub->type = type;
	pub->status = TOPIC_ACTIVE;
	pub->uri = NULL;
	pub->port = 0;
	pub->data = "Hello from stros, here";
	pub->handler = NULL;
	dput(node->publishers, topic,(void*)pub);
	return pub;
}