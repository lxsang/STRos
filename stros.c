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

    if (MATCH("MASTER", "api")) {
        pconfig->master = strdup(value);
    } else if (MATCH("NODE", "remap")) {
        pconfig->id = strdup(value);
    }else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}
à
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
	init_file_system();
}
void stros_stop(int dummy) {
	ros_ok = 0;
	// wait for other thread stop to exit
	//...
	exit(0);
}
void stros_init_node(ros_node_t* node, const char* name, const char* conf)
{
	if(name == NULL) 
	{ 
		LOG("%s", "The node name can not be NULL \n");
		exit(1);
	}
	node->id = NULL;
	node->master = DEFAULT_MASTER;
	node->subscribers = dict();
	node->publishers = dict();
	if(conf != NULL)
		load_config(node,conf);
	ros_ok = 1;
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
		parse_request(client, url);
	close(client);
}
void stros_node_deploy(ros_node_t node)
{
	// ignore the broken PIPE error when writing 
	//or reading to/from a closed socked connection
	signal(SIGPIPE, stros_stop);
	signal(SIGINT, stros_stop);
}
int xmlprc_run()
{
	unsigned port = 0;
	int client_sock = -1;
	int server_sock = -1;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = xmlprc_startup(&port);
	LOG("xmlprc running on port %d\n", port);
	while (stros_ok())
	{
		client_sock = accept(server_sock,(struct sockaddr *)&client_name,&client_name_len);
		if (client_sock == -1)
		{
			perror("Cannot accept client request\n");
			continue;
		}
		/* accept_request(client_sock); */
		if (pthread_create(&newthread , NULL,(void *(*)(void *))accept_request, (void *)client_sock) != 0)
			perror("pthread_create");
		else
		{
			//reclaim the stack data when thread finish
			pthread_detach(newthread) ;
		}
	}

	close(server_sock);

	return(0);
}
int xmlprc_startup(unsigned *port)
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpd == -1)
		error_die("socket");
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("bind");
	if (*port == 0)  /* if dynamically allocating a port */
	{
		socklen_t namelen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
			error_die("getsockname");
		*port = ntohs(name.sin_port);
	}
	if (listen(httpd, 5) < 0)
		error_die("listen");
	return(httpd);
}