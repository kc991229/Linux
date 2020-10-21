#include <stdio.h>
#include <config.h>
int main()
{
    load("my.conf");
    printf("ip=%s\n",get_string("addr"));
    printf("port=%d\n",get_int_default("port",1111));
    return 0;
}
