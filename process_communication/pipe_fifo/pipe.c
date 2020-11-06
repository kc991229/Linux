#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

//查看返回值
void test1()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret < 0)
    {
       perror ("pipe");
    }
    printf("fd[0]=%d,fd[1]=%d\n",fd[0],fd[1]);
}

//测试管道的大小
void test2()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret < 0)
    {
       perror ("pipe");
    }
    int count=0;
    while (1)
    {
        int writesize=0;
        writesize=write(fd[1],"1",1);
        if (writesize < 0)
            break;
        printf("[%d]\n",count++);
    }
    
}

void test3()
{
    int fd[2];
    int ret=pipe(fd);
    if (ret < 0)
        perror("pipe");
    int temp=fork();
    if (temp < 0)
        perror("fork");
    if (temp == 0)
    {
        write(fd[1],"hello",5);
        //child
    }
    else
    {
        //father
        wait(NULL);
        char arr[10];
        read(fd[0],arr,5);
        printf("I read %s\n",arr);
    }
}
int main()
{
    //test1();
    //test2();
    test3();
    return 0;
}
