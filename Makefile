CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=build

OBJS= 	request.o\
 		3rd/ini/ini.o\
		3rd/xml/sxmlc.o\
		response.o\
		utils.o\
		list.o\
		dictionary.o\
		rpc.o\
		stros.o\
		api.o
LIBS=-lpthread -ldl
#-lsocket

main:  client test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: client list dict ros hash

hash: utils.o
	$(CC) $(CFLAGS) list.o utils.o  -o $(BUILDIRD)/hash test/hash.c

client:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/client test/client.c

list: list.o utils.o
	$(CC) $(CFLAGS) list.o utils.o  -o $(BUILDIRD)/test_list test/test_list.c

dict: list.o utils.o dictionary.o
	$(CC) $(CFLAGS)  list.o utils.o dictionary.o -o $(BUILDIRD)/test_dict test/test_dict.c
	
ros: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/sub test/ex_sub.c
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/pub test/ex_pub.c
clean: 
	rm -rf *.o test/*.o $(BUILDIRD)/*
.PRECIOUS: %.o