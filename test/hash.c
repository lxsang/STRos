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
#include "utils.h"

int main (int argc, char const *argv[])
{
	
	printf("Hash of publisherUpdate %d\n", hash("publisherUpdate", HASHSIZE));
	printf("Hash of requestTopic %d\n", hash("requestTopic", HASHSIZE));
	printf("#define TYPE_BOOL %d\n",hash("std_msgs/Bool", HASHSIZE));
	printf("#define TYPE_BYTE %d\n",hash("std_msgs/Byte", HASHSIZE));
	printf("#define TYPE_BYTEMULTIARRAY %d\n",hash("std_msgs/ByteMultiArray", HASHSIZE));
	printf("#define TYPE_CHAR %d\n",hash("std_msgs/Char", HASHSIZE));
	printf("#define TYPE_COLORRGBA %d\n",hash("std_msgs/ColorRGBA", HASHSIZE));
	printf("#define TYPE_DURATION %d\n",hash("std_msgs/Duration", HASHSIZE));
	printf("#define TYPE_EMPTY %d\n",hash("std_msgs/Empty", HASHSIZE));
	printf("#define TYPE_FLOAT32 %d\n",hash("std_msgs/Float32", HASHSIZE));
	printf("#define TYPE_FLOAT32MULTIARRAY %d\n",hash("std_msgs/Float32MultiArray", HASHSIZE));
	printf("#define TYPE_FLOAT64 %d\n",hash("std_msgs/Float64", HASHSIZE));
	printf("#define TYPE_FLOAT64MULTIARRAY %d\n",hash("std_msgs/Float64MultiArray", HASHSIZE));
	printf("#define TYPE_HEADER %d\n",hash("std_msgs/Header", HASHSIZE));
	printf("#define TYPE_INT16 %d\n",hash("std_msgs/Int16", HASHSIZE));
	printf("#define TYPE_INT16MULTIARRAY %d\n",hash("std_msgs/Int16MultiArray", HASHSIZE));
	printf("#define TYPE_INT32 %d\n",hash("std_msgs/Int32", HASHSIZE));
	printf("#define TYPE_INT32MULTIARRAY %d\n",hash("std_msgs/Int32MultiArray", HASHSIZE));
	printf("#define TYPE_INT64 %d\n",hash("std_msgs/Int64", HASHSIZE));
	printf("#define TYPE_INT64MULTIARRAY %d\n",hash("std_msgs/Int64MultiArray", HASHSIZE));
	printf("#define TYPE_INT8 %d\n",hash("std_msgs/Int8", HASHSIZE));
	printf("#define TYPE_INT8MULTIARRAY %d\n",hash("std_msgs/Int8MultiArray", HASHSIZE));
	printf("#define TYPE_MULTIARRAYDIMENSION %d\n",hash("std_msgs/MultiArrayDimension", HASHSIZE));
	printf("#define TYPE_MULTIARRAYLAYOUT %d\n",hash("std_msgs/MultiArrayLayout", HASHSIZE));
	printf("#define TYPE_STRING %d\n",hash("std_msgs/String", HASHSIZE));
	printf("#define TYPE_TIME %d\n",hash("std_msgs/Time", HASHSIZE));
	printf("#define TYPE_UINT16 %d\n",hash("std_msgs/UInt16", HASHSIZE));
	printf("#define TYPE_UINT16MULTIARRAY %d\n",hash("std_msgs/UInt16MultiArray", HASHSIZE));
	printf("#define TYPE_UINT32 %d\n",hash("std_msgs/UInt32", HASHSIZE));
	printf("#define TYPE_UINT32MULTIARRAY %d\n",hash("std_msgs/UInt32MultiArray", HASHSIZE));
	printf("#define TYPE_UINT64 %d\n",hash("std_msgs/UInt64", HASHSIZE));
	printf("#define TYPE_UINT64MULTIARRAY %d\n",hash("std_msgs/UInt64MultiArray", HASHSIZE));
	printf("#define TYPE_UINT8 %d\n",hash("std_msgs/UInt8", HASHSIZE));
	printf("#define TYPE_UINT8MULTIARRAY %d\n",hash("std_msgs/UInt8MultiArray", HASHSIZE));
	return 0;
}