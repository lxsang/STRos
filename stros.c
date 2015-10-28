#include "stros.h"

static int server_sock = -1;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

static int config_handler(void* conf, const char* section, const char* name,
                   const char* value)
{
    config_t* pconfig = (config_t*)conf;

    if (MATCH("SERVER", "port")) {
        pconfig->port = atoi(value);
    } else if (MATCH("SERVER", "master_url")) {
        pconfig->master_url = strdup(value);
    } else if (MATCH("SERVER", "master_port")) {
        pconfig->master_port = atoi(value);
    }else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void load_config(const char* file)
{
	server_config.port = 0;
	server_config.master_url = "127.0.0.1";
	server_config.master_port = 3113;
	if (ini_parse(file, config_handler, &server_config) < 0) {
		LOG("Can't load '%s'\n. Used defaut configuration", file);
	}
	else
	{
		LOG("Using configuration : %s\n", file);
	}
	// if needed
	init_file_system();
}
void stop_serve(int dummy) {
	if(server_sock > 0)
		close(server_sock);
	exit(0);
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

int main(int argc, char* argv[])
{
// load the config first
	if(argc==1)
		load_config(CONFIG);
	else
		load_config(argv[1]);

	unsigned port = server_config.port;
	int client_sock = -1;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	// ignore the broken PIPE error when writing 
	//or reading to/from a closed socked connection
	signal(SIGPIPE, SIG_IGN);
	signal(SIGABRT, SIG_IGN);
	signal(SIGINT, stop_serve);
	server_sock = startup(&port);
	LOG("httpd running on port %d\n", port);

	while (1)
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
int startup(unsigned *port)
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