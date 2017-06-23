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
#include "utils.h"

//define all basic mime here

static mime_t _mimes[] = {
	{"image/bmp",(const char *[]){"bmp",NULL},1},
	{"image/jpeg",(const char *[]){"jpg","jpeg",NULL},1},
	{"text/css",(const char *[]){"css",NULL},0},
	{"text/csv",(const char *[]){"csv",NULL},0},
	{"application/pdf",(const char *[]){"pdf",NULL},1},
	{"image/gif",(const char *[]){"gif",NULL},1},
	{"text/html",(const char *[]){"html","htm",NULL},0},
	{"application/json",(const char *[]){"json",NULL},0},
	{"application/javascript",(const char *[]){"js",NULL},0},
	{"image/png",(const char *[]){"png",NULL},1},
	{"image/x-portable-pixmap",(const char *[]){"ppm",NULL},1},
	{"application/x-rar-compressed",(const char *[]){"rar",NULL},1},
	{"image/tiff",(const char *[]){"tiff",NULL},1},
	{"application/x-tar",(const char *[]){"tar",NULL},1},
	{"text/plain",(const char *[]){"txt",NULL},0},
	{"application/x-font-ttf",(const char *[]){"ttf",NULL},1},
	{"application/xhtml+xml",(const char *[]){"xhtml",NULL},0},
	{"application/xml",(const char *[]){"xml",NULL},0},
	{"application/zip",(const char *[]){"zip",NULL},1},
	{"image/svg+xml",(const char *[]){"svg",NULL},0},
	{"application/vnd.ms-fontobject",(const char *[]){"eot",NULL},1},
	{"application/x-font-woff",(const char *[]){"woff","woff2",NULL},1},
	{"application/x-font-otf",(const char *[]){"otf",NULL},1},
	{"audio/mpeg",(const char *[]){"mp3","mpeg",NULL},1},
	{NULL,NULL,0}
};


char* __s(const char* fstring,...)
{
	char* data;
	va_list arguments; 
	int dlen;
	va_start( arguments, fstring);
    dlen = vsnprintf(0,0,fstring,arguments) + 1;
    va_end(arguments); 
    va_end(arguments); 
    if ((data = (char*)malloc(dlen*sizeof(char))) != 0)
    {
        va_start(arguments, fstring);
        vsnprintf(data, dlen, fstring, arguments);
        va_end(arguments);
        return data;
    } else
    	return "";
}


/**
 * Trim a string by a character on both ends
 * @param str   The target string
 * @param delim the delim
 */
void trim(char* str, const char delim)
{
    char * p = str;
    int l = strlen(p);

    while(p[l - 1] == delim) p[--l] = 0;
    while(* p && (* p) == delim ) ++p, --l;
    memmove(str, p, l + 1);
}

void removeAll(const char* path,int mode)
{
	DIR           *d;
	struct dirent *dir;
	char* file;
	struct stat st;
	if( stat(path, &st) == 0)
	{
		if(S_ISDIR(st.st_mode))
		{
			d = opendir(path);
			if(d)
			{
				while ((dir = readdir(d)) != NULL)
				{
					if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..")==0) continue;
		  			file = __s("%s/%s",path,dir->d_name);

		  			removeAll(file,1);
		  			free(file);
				}
				closedir(d);
			}
		}
		if(mode)
			remove(path);
	}
	
}

char* __time(time_t time)
{
	struct tm *t = localtime(&time);
	char * buf = asctime(t);
	char* pos = strchr(buf,'\n');
	if(pos) *pos = '\0';
	return buf;
}
char* server_time()
{
	return __time(time(NULL));
}

/**
 * Get extension of a file name
 * @param  file The file name
 * @return      the extension
 */
char* ext(const char* file)
{
	char* token,*ltoken = "";
	if(file == NULL) return NULL;
	char* str_cpy = strdup(file);
	if(strstr(str_cpy,".")<= 0) return "";
	if(*file == '.')
		trim(str_cpy,'.');

	while((token = strsep(&str_cpy,".")) && strlen(token)>0) {ltoken = token;}
	free(str_cpy);
	return ltoken;

}
/*get mime file info from extension*/
mime_t mime_from_ext(const char* ex)
{
	for(int i = 0; _mimes[i].type != NULL; i++)
		for(int j = 0; _mimes[i].ext[j] != NULL; j++)
		{
			if(IEQU(ex,_mimes[i].ext[j])) return _mimes[i];
		}
	
	return (mime_t){"application/octet-stream",(const char *[]){ext,NULL},1};
}
/*get mime file info from type*/
mime_t mime_from_type(const char* type)
{
	for(int i = 0; _mimes[i].type != NULL; i++)
		if(strstr(type, _mimes[i].type) != NULL) 
	   		return _mimes[i];
	return (mime_t){NULL,NULL,0};
}
/**
 * Get correct HTTP mime type of a file
 * This is a minimalistic mime type list supported
 * by the server
 * @param  file File name
 * @return      The HTTP Mime Type
 */
char* mime(const char* file)
{
	char * ex = ext(file);
	mime_t m = mime_from_ext(ex);
	return m.type;
}

int is_bin(const char* file)
{
	char * ex = ext(file);
	mime_t m = mime_from_ext(ex);
	return m.bin;
}

int match_int(const char* search)
{
	return regex_match("^[-+]?[0-9]+$",search);
}
int match_float(const char* search)
{
	return regex_match("^[+-]?[0-9]*\\.[0-9]+$",search);
}
int regex_match(const char* expr,const char* search)
{
	regex_t regex;
    int reti;
    char msgbuf[100];
    int ret;
	/* Compile regular expression */
    reti = regcomp(&regex, expr, REG_ICASE | REG_EXTENDED);
    if( reti ){ 
    	LOG("Could not compile regex: %s\n",expr);
    	return 0; 
    }

	/* Execute regular expression */
    reti = regexec(&regex, search, 0, NULL, 0);
    if( !reti ){
            //LOG("Match");
            ret = 1;
    }
    else if( reti == REG_NOMATCH ){
            //LOG("No match");
            ret = 0;
    }
    else{
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        LOG("Regex match failed: %s\n", msgbuf);
        ret = 0;
    }

	
	regfree(&regex);
	return ret;
}
char *url_decode(const char *str) {
	char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;

	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}

char *url_encode(const char *str) {
  char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else if (*pstr == ' ') 
      *pbuf++ = '+';
    else 
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}
unsigned hash(const char* key, int hash_size)
{
	unsigned hashval = simple_hash(key);
    return hashval % hash_size;	
}
unsigned simple_hash(const char* key)
{
	unsigned hashval;
    for (hashval = 0; *key != '\0'; key++)
      hashval = *key + 31 * hashval;
	return hashval;
}
int _exist(const char* f)
{
	struct stat st;
	return !(stat(f, &st) == -1);
}
int is_file(const char* f)
{
	int st = is_dir(f);
	if(st == -1) return -1;
	else return st==0;
}
int is_dir(const char* f)
{
	struct stat st;
	if(stat(f, &st) == -1)
		return -1; // unknow
	else if(st.st_mode & S_IFDIR)
		return 1;
	else 
		return 0;
}
// These vars will contain the hash
 