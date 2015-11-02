#include "../stros.h"

void callback(void* data)
{
	char* msg = (char*) data;
	LOG("Receive : %s\n", msg);
}

int main (int argc, char const *argv[])
{
	ros_node_t* node = stros_init_node("/dummy",argv[1]);
	// subscribe
	subscribe_to( node,"/chatter",MSG_STRING,callback);
	stros_node_deploy(node);
	printf("Finish deploy\n");
	spin();
	return 0;
}