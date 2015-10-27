CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=build

OBJS=request.o 3rd/ini/ini.o 3rd/xml/sxmlc.c response.o utils.o
LIBS=-lpthread -ldl
#-lsocket

main: stros test


stros:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/stros stros.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BUILDIRD)/client test/client.c


clean: 
	rm -rf *.o test/*.o $(BUILDIRD)/*
.PRECIOUS: %.o