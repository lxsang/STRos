CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=build
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PF_FLAG=-D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
	PF_FLAG=
endif
CFLAGS=-W -Wall -g -std=c99 -D DEBUG $(PF_FLAG)
UTILS = utils.o\
		list.o \
		dictionary.o 

OBJS= 	list.o \
		dictionary.o \
		utils.o\
		request.o\
 		3rd/ini/ini.o\
		3rd/xml/sxmlc.o\
		response.o\
		rpc.o\
		stros.o\
		api.o
		
LIBS=-lpthread -ldl
#-lsocket

main:  client test

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

test: client list dict ros hash

hash: $(UTILS)
	$(CC) $(CFLAGS) $(UTILS)  -o $(BUILDIRD)/hash test/hash.c

client:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)   -o $(BUILDIRD)/client test/client.c

list: $(UTILS)
	$(CC) $(CFLAGS) $(UTILS)   -o $(BUILDIRD)/test_list test/test_list.c

dict: $(UTILS)
	$(CC) $(CFLAGS)  $(UTILS)  -o $(BUILDIRD)/test_dict test/test_dict.c
	
ros: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/sub test/ex_sub.c
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)   -o $(BUILDIRD)/pub test/ex_pub.c
clean: 
	rm -rf *.o test/*.o $(BUILDIRD)/*
.PRECIOUS: %.o