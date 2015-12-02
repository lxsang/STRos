CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=build
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLUGINS_BASE=/root/workspace/ant-http/plugins
	PF_FLAG=-D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
	PLUGINS_BASE=/Users/mrsang/Documents/ushare/cwp/ant-http/plugins
	PF_FLAG=
endif
CFLAGS=-W -Wall -g -std=c99 -D DEBUG $(PF_FLAG)
PLUGINSDEP = 	$(PLUGINS_BASE)/list.o \
				$(PLUGINS_BASE)/dictionary.o \
				$(PLUGINS_BASE)/utils.o

OBJS= 	request.o\
 		3rd/ini/ini.o\
		3rd/xml/sxmlc.o\
		response.o\
		rpc.o\
		stros.o\
		api.o \
		$(PLUGINSDEP)
LIBS=-lpthread -ldl
#-lsocket

main:  client test

%.o: %.c
	$(CC) $(CFLAGS) -I$(PLUGINS_BASE) -c $< -o $@

test: client list dict ros hash

hash: $(PLUGINSDEP)
	$(CC) $(CFLAGS) $(PLUGINSDEP) -I$(PLUGINS_BASE)  -o $(BUILDIRD)/hash test/hash.c

client:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -I$(PLUGINS_BASE)  -o $(BUILDIRD)/client test/client.c

list: $(PLUGINSDEP)
	$(CC) $(CFLAGS) $(PLUGINSDEP) -I$(PLUGINS_BASE)  -o $(BUILDIRD)/test_list test/test_list.c

dict: $(PLUGINSDEP)
	$(CC) $(CFLAGS)  $(PLUGINSDEP) -I$(PLUGINS_BASE) -o $(BUILDIRD)/test_dict test/test_dict.c
	
ros: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -I$(PLUGINS_BASE)  -o $(BUILDIRD)/sub test/ex_sub.c
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -I$(PLUGINS_BASE)  -o $(BUILDIRD)/pub test/ex_pub.c
clean: 
	rm -rf *.o test/*.o $(BUILDIRD)/*
.PRECIOUS: %.o