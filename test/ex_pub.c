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
#include "../stros.h"

int main (int argc, char const *argv[])
{
	signal(SIGINT, stros_stop);
	ros_node_t* node = stros_init_node("/dummy",argv[1]);
	// subscribe
	publisher* pub = create_publisher(node, "/talker", MSG_STRING);
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