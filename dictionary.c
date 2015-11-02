#include "dictionary.h"

dictionary dict()
{
	dictionary d = (dictionary)malloc(HASHSIZE*sizeof(association));
	for(int i=0; i< HASHSIZE;i++)
		d[i] = NULL;
	return d;
}
association dlookup(dictionary dic,const char* key)
{
	association np;
	if(dic == NULL) return NULL;
    for (np = dic[hash(key,DHASHSIZE)]; np != NULL; np = np->next)
    {
    	if(!np || !np->key)
    	{
    		return NULL;
    	}
        if (strcmp(key, np->key) == 0)
          return np; /* found */
    }
    return NULL; /* not found */
}
association __put_el_with_key(dictionary dic, const char* key)
{
	association np;
    unsigned hashval;
	if(dic == NULL) return NULL;
    if ((np = dlookup(dic,key)) == NULL) { /* not found */
        np = (association) malloc(sizeof(*np));
        if (np == NULL || (np->key = strdup(key)) == NULL)
          return NULL;
        hashval = hash(key, DHASHSIZE);
        np->next = dic[hashval];
        dic[hashval] = np;
    }
    return np;
}
association dput(dictionary dic,const char* key, void* value)
{
	association np = __put_el_with_key(dic,key);
	if(np == NULL) return NULL;
	np->value = value;
    return np;
}
int dremove(dictionary dic, const char* key)
{
	if(dic == NULL) return 0;
	int hashval = hash(key, DHASHSIZE);
	association np = dic[hashval];
	if(np!=NULL && strcmp(key,np->key)==0)
	{
		dic[hashval] = np->next;
		return 1;
	}
    for (np= dic[hashval]; np != NULL; np = np->next)
        if (np->next!=NULL&&strcmp(key, np->next->key) == 0)
        {
         	np->next = np->next->next; /* found */
         	return 1;
        }
    return 0; /* not found */

}
void* dvalue(dictionary dic, const char* key)
{
	association as = dlookup(dic,key);
	if(as == NULL) return NULL;
	return as->value;
}

void freedict(dictionary dic){free(dic);}

