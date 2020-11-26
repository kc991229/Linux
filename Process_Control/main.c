#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
void manu()
{
  printf("**************************\n");
  printf("0:sign out\n\n");
  printf("1:Whether it is leap year or not\n\n");
  printf("2:Find the greatest common divisor of two numbers\n\n");
  printf("3:Print system date\n\n");
  printf("**************************\n");
}

int main()
{
  manu();
  int num=1;
  while (1)
{
  scanf("%d",&num);

  while (!(num>=0 && num <4))
{
  printf("please enter right commond\n");
  scanf("%d",&num);
}

  if (num==0)
    break;

  pid_t pid=0;
  pid=fork();

  if( pid==-1)
    perror("fork()"),exit(1);
  else if (pid==0)
   {
    switch(num)
    {
     case 1:execl("judge_year","judge_year",NULL);break;
     case 2:execl("com_div","com_div",NULL);break;
     case 3:execl("/bin/date","date",NULL);break; 
    }
    _exit(0);
   }
  else
   {
    int st;
    wait(&st);  
    manu();
   }
}
  return 0;
}
