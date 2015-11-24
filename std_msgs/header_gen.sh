#!/bin/bash

for file in ./*
do
	f=${file##*/}
	#echo "#define ROS_MD5_${f%.h} \"\"" | awk '{print toupper($0)}'
	#echo "#define `echo "MSG_${f%.h}" | awk '{print toupper($0)}'` \"std_msgs/${f%.h}\""
	
	echo "printf(\"#define `echo "TYPE_${f%.h}" | awk '{print toupper($0)}'` %d\\n\",hash(\"std_msgs/${f%.h}\"));"
done