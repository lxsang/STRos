# STRos
Minimal ROS compatible client for small embeded systems (support Linux).
## Feature
- Lightweight and independent, can work without ros installed
- Simple way to create node, public or subscibe to a topic.
- At the moment, the client supports only the standard message types (std_msgs).

## Example
This is how the chatter example (see on ros tutorial's page) is implemented using our API
### publisher
```c
// include the api
#include "stros.h"

int main (int argc, char const *argv[])
{
  // create new node, the argv[1] is the parameter file
  // that is used to configure the node using the INI syntax 
  // (see below)
	ros_node_t* node = stros_init_node("/dummy",argv[1]);
	// create new publisher
	publisher* pub = create_publisher(node, "/chatter", MSG_STRING);
	// deploy the node to the system
	stros_node_deploy(node);
	
	char* msg;
	while(stros_ok())
	{
		msg = __s("Hello from STRos it's now : %u",(unsigned)time(NULL));
		LOG("Publishing : %s\n", msg);
		// publish a message and wait for 1 second
		publish(pub,(void*)msg);
		sleep(1);
	}
	return 0;
}
```
### subscriber
```c
#include "stros.h"
/*
  The callback function which will be called
  when the subscriber receives data from the 
  topic
*/
void callback(void* data)
{
	char* msg = (char*) data;
	LOG("Receive : %s\n", msg);
}

int main (int argc, char const *argv[])
{
	ros_node_t* node = stros_init_node("/dummy",argv[1]);
	// subscribe topic /chatter
	subscribe_to( node,"/chatter",MSG_STRING,callback);
	stros_node_deploy(node);
	// enter loop and wait for comming data
	spin();
	return 0;
}
```
## ROS node configuration using INI file
The node parameters can be easily reconfigured using a config file in INI format. The following parameters are supported:
- The Master's URI
- The Master's port
- The node name

For example
```
[MASTER]             
uri  = 192.168.10.100 ;
port = 11311 ;
[NODE]             
remap = /zombie ;
```
usage :
```
./talker config.ini
```
## Future work
- Create a smalltalk wrapper for the api (based on LitleSmalltalk), so the node can be easily and dinamically created (without recompilation)
- Support user defined message types.
