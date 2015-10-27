#include "../request.h"
#define TEST_XML "<?xml version=\"1.0\"?>\r\n\
					<methodCall>\r\n\
  					  	<methodName>getSystemState</methodName>\r\n\
  						  <params>\r\n\
						 	  <param><value><string>/phantom</string></value></param>\r\n\
  						</params> \r\n\
					</methodCall>"
int main (int argc, char const *argv[])
{
	if(argc != 3)
	{
		printf("Usage %s <ip> <port>\n",argv[0]);
		return 0;
	}
	/*printf("array: %d\n", hash("array"));
	printf("base64: %d\n", hash("base64"));
	printf("bool: %d\n", hash("boolean"));
	printf("date: %d\n", hash("dateTime.iso8601"));
	printf("double: %d\n", hash("double"));
	printf("int: %d\n", hash("int"));
	printf("i4: %d\n", hash("i4"));
	printf("string: %d\n", hash("string"));
	printf("nil: %d\n", hash("nil"));*/
	char* data = send_post_request(argv[1], atoi(argv[2]), TEST_XML);
	
	printf("RECEIVE:\n%s\n", data);
	return 0;
}