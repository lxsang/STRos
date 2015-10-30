#include "../list.h"
#include "../dictionary.h"
int main (int argc, char const *argv[])
{
	dictionary d = dict();
	int i = 45;
	dput(d,"int", &i);
	char* doc = "xml doc";
	dput(d,"doc", doc);
	list l1 = split("12,-34.567,this is a string",",");
	dput(d,"list", l1);
	//association s;
	
	printf("Int: %d\n",*((int*)dvalue(d,"int")));
	printf("doc: %s\n",(char*)dvalue(d,"doc"));
	printf("list: [%s]\n",as_string((list)dvalue(d,"list")));
	
	freedict(d);
	list_free(&l1);
	return 0;
}