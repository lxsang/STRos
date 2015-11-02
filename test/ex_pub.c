#include "../stros.h"

int main (int argc, char const *argv[])
{
	ros_node_t* node = stros_init_node("/dummy",argv[1]);
	// subscribe
	publisher* pub = create_publisher(node, "/sender", MSG_STRING);
	stros_node_deploy(node);
	
	char* msg;
	//spin();
	while(stros_ok())
	{
		msg = __s("Hello from STRos it's now : %u",(unsigned)time(NULL));
		LOG("Publishing : %s\n", msg);
		publish(pub,(void*)msg);
		sleep(1);
	}
	return 0;
}