#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "utils.h"
#define for_each_assoc(assoc, dic) \
    for(int i = 0; i < HASHSIZE; i++) \
    	for(assoc = dic[i];assoc!= NULL; assoc = assoc->next)

/**
 * Dictionary for header
 */
typedef struct  __assoc{ 
    struct __assoc *next; 
    char *key; 
    void* value;
    //char *value;
} * association;

typedef  association* dictionary;
dictionary dict();
association dlookup(dictionary,const char*);
void* dvalue(dictionary, const char*);
association dput(dictionary,const char*, void*);
int dremove(dictionary, const char*);
void freedict(dictionary);
void stest(const char* );

#endif