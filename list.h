#ifndef LIST_H
#define LIST_H
#include "utils.h"
#define list item
typedef struct __item{
	int type;
	union{
		int 			i;
		int 			b;
		char* 			s;
		double 			d;
		char* 			date;
		char* 			b64;
		struct __item* 	array;
	} value;
	struct __item* next;
}*item;

list list_init();
void list_put(list*,item);
void list_put_i(list*,int);
void list_put_d(list*,double);
void list_put_b(list*,int);
void list_put_b64(list*,const char*);
void list_put_date(list*,const char*);
void list_put_s(list*,const char*);
void list_put_array(list*,list);
item list_last(list);
int list_remove(list,int);
int list_size(list);
item list_at(list,int);
int list_empty(list);
item list_item(int type);
list split(const char*, const char*);
char* as_string(list);
void list_put_special(list*, const char*);
void list_free(list *);
#endif