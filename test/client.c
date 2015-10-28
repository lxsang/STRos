#include "../request.h"

int main (int argc, char const *argv[])
{
	if(argc != 3)
	{
		printf("Usage %s <ip> <port>\n",argv[0]);
		return 0;
	}
	printf("%s\n", get_ip_address());
	rpc_request_t* rq = malloc(sizeof(rpc_request_t));
	rq->method = "getSystemState";
	rq->params = list_init();
	list_put_s(&rq->params,"/phantom");
	char* xml = gen_rpc_call(rq);
	printf("%s\n", xml);
	rpc_response_t* data = send_post_request(argv[1], atoi(argv[2]), xml);
	
	printf("RECEIVE:\n");
	printf("%s\n", list_to_xml_params(data->data));
	
	free_request(rq);
	free_response(data);
	return 0;
}