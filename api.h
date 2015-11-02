#ifndef API_H
#define API_H
#include "request.h"
#include "dictionary.h"
//#include "list.h"

/** define MD5 for ROS standard message*/
#define ROS_MD5_BOOL 				"8b94c1b53db61fb6aed406028ad6332a"
#define ROS_MD5_BYTE 				"ad736a2e8818154c487bb80fe42ce43b"
#define ROS_MD5_BYTEMULTIARRAY 		"70ea476cbcfd65ac2f68f3cda1e891fe"
#define ROS_MD5_CHAR 				"1bf77f25acecdedba0e224b162199717"
#define ROS_MD5_COLORRGBA 			"a29a96539573343b1310c73607334b00"
#define ROS_MD5_DURATION 			"3e286caf4241d664e55f3ad380e2ae46"
#define ROS_MD5_EMPTY 				"d41d8cd98f00b204e9800998ecf8427e"
#define ROS_MD5_FLOAT32 			"73fcbf46b49191e672908e50842a83d4"
#define ROS_MD5_FLOAT32MULTIARRAY 	"6a40e0ffa6a17a503ac3f8616991b1f6"
#define ROS_MD5_FLOAT64 			"fdb28210bfa9d7c91146260178d9a584"
#define ROS_MD5_FLOAT64MULTIARRAY 	"4b7d974086d4060e7db4613a7e6c3ba4"
#define ROS_MD5_HEADER 				"2176decaecbce78abc3b96ef049fabed"
#define ROS_MD5_INT16 				"8524586e34fbd7cb1c08c5f5f1ca0e57"
#define ROS_MD5_INT16MULTIARRAY 	"d9338d7f523fcb692fae9d0a0e9f067c"
#define ROS_MD5_INT32 				"da5909fbe378aeaf85e547e830cc1bb7"
#define ROS_MD5_INT32MULTIARRAY 	"1d99f79f8b325b44fee908053e9c945b"
#define ROS_MD5_INT64 				"34add168574510e6e17f5d23ecc077ef"
#define ROS_MD5_INT64MULTIARRAY 	"54865aa6c65be0448113a2afc6a49270"
#define ROS_MD5_INT8 				"27ffa0c9c4b8fb8492252bcad9e5c57b"
#define ROS_MD5_INT8MULTIARRAY 		"d7c1af35a1b4781bbe79e03dd94b7c13"
#define ROS_MD5_MULTIARRAYDIMENSION "4cd0c83a8683deae40ecdac60e53bfa8"
#define ROS_MD5_MULTIARRAYLAYOUT 	"0fed2a11c13e11c5571b4e2a995a91a3"
#define ROS_MD5_STRING 				"992ce8a1687cec8c8bd883ec73ca41d1"
#define ROS_MD5_TIME 				"cd7166c74c552c311fbcc2fe5a7bc289"
#define ROS_MD5_UINT16 				"1df79edf208b629fe6b81923a544552d"
#define ROS_MD5_UINT16MULTIARRAY 	"52f264f1c973c4b73790d384c6cb4484"
#define ROS_MD5_UINT32 				"304a39449588c7f8ce2df6e8001c5fce"
#define ROS_MD5_UINT32MULTIARRAY 	"4d6a180abc9be191b96a7eda6c8a233d"
#define ROS_MD5_UINT64 				"1b2a79973e8bf53d7b53acb71299cb57"
#define ROS_MD5_UINT64MULTIARRAY 	"6088f127afb1d6c72927aa1247e945af"
#define ROS_MD5_UINT8 				"7c8164229e7d2c17eb95e9231617fdee"
#define ROS_MD5_UINT8MULTIARRAY 	"82373f1612381bb6ee473b5cd6f5d89c"

/**Msg define*/
#define MSG_BOOL 					"std_msgs/Bool"
#define MSG_BYTE 					"std_msgs/Byte"
#define MSG_BYTEMULTIARRAY 			"std_msgs/ByteMultiArray"
#define MSG_CHAR 					"std_msgs/Char"
#define MSG_COLORRGBA 				"std_msgs/ColorRGBA"
#define MSG_DURATION 				"std_msgs/Duration"
#define MSG_EMPTY 					"std_msgs/Empty"
#define MSG_FLOAT32 				"std_msgs/Float32"
#define MSG_FLOAT32MULTIARRAY 		"std_msgs/Float32MultiArray"
#define MSG_FLOAT64 				"std_msgs/Float64"
#define MSG_FLOAT64MULTIARRAY 		"std_msgs/Float64MultiArray"
#define MSG_HEADER 					"std_msgs/Header"
#define MSG_INT16 					"std_msgs/Int16"
#define MSG_INT16MULTIARRAY 		"std_msgs/Int16MultiArray"
#define MSG_INT32 					"std_msgs/Int32"
#define MSG_INT32MULTIARRAY 		"std_msgs/Int32MultiArray"
#define MSG_INT64 					"std_msgs/Int64"
#define MSG_INT64MULTIARRAY 		"std_msgs/Int64MultiArray"
#define MSG_INT8 					"std_msgs/Int8"
#define MSG_INT8MULTIARRAY 			"std_msgs/Int8MultiArray"
#define MSG_MULTIARRAYDIMENSION 	"std_msgs/MultiArrayDimension"
#define MSG_MULTIARRAYLAYOUT 		"std_msgs/MultiArrayLayout"
#define MSG_STRING 					"std_msgs/String"
#define MSG_TIME 					"std_msgs/Time"
#define MSG_UINT16 					"std_msgs/UInt16"
#define MSG_UINT16MULTIARRAY 		"std_msgs/UInt16MultiArray"
#define MSG_UINT32 					"std_msgs/UInt32"
#define MSG_UINT32MULTIARRAY 		"std_msgs/UInt32MultiArray"
#define MSG_UINT64 					"std_msgs/UInt64"
#define MSG_UINT64MULTIARRAY 		"std_msgs/UInt64MultiArray"
#define MSG_UINT8 					"std_msgs/UInt8"
#define MSG_UINT8MULTIARRAY 		"std_msgs/UInt8MultiArray"
/*connection header type*/
#define TYPE_BOOL 					715
#define TYPE_BYTE 					233
#define TYPE_BYTEMULTIARRAY 		617
#define TYPE_CHAR 					823
#define TYPE_COLORRGBA 				982
#define TYPE_DURATION 				245
#define TYPE_EMPTY 					396
#define TYPE_FLOAT32 				570
#define TYPE_FLOAT32MULTIARRAY	 	634
#define TYPE_FLOAT64 				665
#define TYPE_FLOAT64MULTIARRAY 		25
#define TYPE_HEADER 				558
#define TYPE_INT16 					659
#define TYPE_INT16MULTIARRAY 		915
#define TYPE_INT32 					717
#define TYPE_INT32MULTIARRAY 		845
#define TYPE_INT64 					812
#define TYPE_INT64MULTIARRAY 		236
#define TYPE_INT8 					522
#define TYPE_INT8MULTIARRAY 		586
#define TYPE_MULTIARRAYDIMENSION 	677
#define TYPE_MULTIARRAYLAYOUT 		587
#define TYPE_STRING 				946
#define TYPE_TIME 					686
#define TYPE_UINT16 				640
#define TYPE_UINT16MULTIARRAY 		832
#define TYPE_UINT32 				698
#define TYPE_UINT32MULTIARRAY 		762
#define TYPE_UINT64 				793
#define TYPE_UINT64MULTIARRAY 		153
#define TYPE_UINT8 					125
#define TYPE_UINT8MULTIARRAY 		253

#define RQ_PUBLISHER_UPDATE 		485
#define RQ_REQUEST_TOPIC 			992

#define TOPIC_ACTIVE 	1
#define TOPIC_DOWN		0
#define TOPIC_REFRESH	2


typedef struct{
	char* id;
	char* api;
	dictionary subscribers;
	dictionary publishers;
	char* master_uri;
	int master_port;
} ros_node_t;

typedef struct{
	char* callerid;
	char* topic;
	char* type;
	char* uri;
	int port;
	int status;
	void* data;
	void (*handler)(void*);
} topic_t;
#define subscriber topic_t
#define publisher  topic_t
typedef struct{
	dictionary header;
	void* data;
} ros_tcp_t;

typedef struct {
	int pid;
	ros_node_t* node;
	int server_sock;
	int thread_id;
} server_t;

typedef struct{
	publisher* pub;
	int client;
} tcp_pub_t;
server_t xml_prc_server;
pthread_mutex_t node_mux;
int ros_ok;

void xml_rpc_api_handler(int,void*);
void publisherUpdate(rpc_value_t , int, int);
void requestTopic(rpc_value_t, int, int );
void tcp_ros_request(const char* , int ,subscriber*);
int gen_tcp_header_for(subscriber*, char*);
char* md5sum_of(const char*);
int verify_ros_header(subscriber*, char*, int);
void* ros_data(char*,int);
char* ros_string(char*);
void extract_url(subscriber*, list);
void publish_tcp_data(void*);
int tcp_send_data(topic_t* , int);
int tcp_send_string(topic_t*, int);
int stros_ok();
/*
* subscribe to a to pic
*/
void stros_toggle(int);
#endif