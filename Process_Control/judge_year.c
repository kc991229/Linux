#include "stdio.h"

int main()
{
  int year=0;
  scanf("%d",&year);
  while (year<0)
   {
    printf("please enter right year\n");
    scanf("%d",&year);
   }
  if ((year%4==0 && year%100!=0) || year%400==0)
    printf("%d is leap year\n",year);
  else
    printf("%d is not leap year\n",year);
  return 0;
}
