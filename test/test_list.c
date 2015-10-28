#include "../list.h"

int main (int argc, char const *argv[])
{
	list l = list_init();
	printf("empty list : %s\n", as_string(l));
	// add some element
	//0
	list_put_i(&l,45);//0
	list_put_s(&l,"hello");//1
	list_put_date(&l,"dateTime.iso8601");//2
	list_put_b64(&l,"G48GFGFGF3FGFG3");
	list_put_d(&l,-1.2365);
	list_put_b(&l, true);
	list l1 = split("12,-34.567,this is a string",",");
	list_put_array(&l,l1);
	printf("list size: %d\n", list_size(l));
	printf("List : %s\n", as_string(l));
	printf("List at 6 %s\n",as_string(list_at(l,6)));
	//try to remove an item
	list_remove(l,3);
	printf("list size: %d\n", list_size(l));
	printf("List : %s\n", as_string(l));
	printf("List at 3 %s\n",as_string(list_at(l,3)));
	printf("Last item %s\n",as_string(list_last(l)));
	list_free(&l);
	return 0;
}