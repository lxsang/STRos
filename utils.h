#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <regex.h>
#include <time.h>

#define EQU(a,b) (strcmp(a,b) == 0)
#define IEQU(a,b) (strcasecmp(a,b) == 0)
#define IS_INT(a) (match_int(a))
#define IS_FLOAT(a) (match_float(a))
#define DIR_SEP "/"
#define true 1
#define false 0
#ifdef DEBUG
	#define LOG(a,...) printf("%s:%d: " a, __FILE__, \
		__LINE__, ##__VA_ARGS__)
#else
    #define LOG(a,...) do{}while(0)
#endif
#define	BUFFLEN 1024
#define HASHSIZE 1024
			
#define RPC_TYPE_ARRAY	601//hash("array")
#define RPC_TYPE_BASE64	335//hash("base64")
#define RPC_TYPE_BOOL	40//hash("boolean")
#define RPC_TYPE_DOUBLE	977//hash("double")
#define RPC_TYPE_DATE	49//hash("dateTime.iso8601")
#define RPC_TYPE_INT	1007//hash("int")
#define RPC_TYPE_I4		235//hash("i4")
#define RPC_TYPE_STRING	17//hash("string")
#define RPC_TYPE_NIL	529//hash("nil")
			
char* __s(const char*,...);
void trim(char*,const char);
void removeAll(const char* path,int mode);
char* __time(time_t time);
char* server_time();
char* ext(const char*);
char* mime(const char*);
int is_bin(const char*);
int match_int(const char*);
int match_float(const char*);
int regex_match(const char*,const char*);
char *url_decode(const char *str);
char *url_encode(const char *str);
char from_hex(char ch);
char to_hex(char code);
unsigned hash(const char*);
#endif
