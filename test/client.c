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