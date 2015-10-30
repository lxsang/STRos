CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=build

OBJS=request.o 3rd/ini/ini.o 3rd/xml/sxmlc.o response.o utils.o rpc.o list.o
LIBS=-lpthread -ldl
#-lsocket

main: stros client


stros:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/stros stros.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

client:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/client test/client.c

list: list.o utils.o
	$(CC) $(CFLAGS) list.o utils.o  -o $(BUILDIRD)/test_list test/test_list.c

dict: list.o utils.o dictionary.o
	$(CC) $(CFLAGS)  list.o utils.o dictionary.o -o $(BUILDIRD)/test_dict test/test_dict.c
clean: 
	rm -rf *.o test/*.o $(BUILDIRD)/*
.PRECIOUS: %.o