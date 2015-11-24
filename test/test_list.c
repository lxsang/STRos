/*
The MIT License (MIT)

Copyright (c) 2015 LE Xuan Sang xsang.le@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "list.h"

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