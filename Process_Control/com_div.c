#include "stdio.h"

int main()
{
  int k=0;
  int i=0,j=0;
  scanf("%d%d",&i,&j);
  if (i>j)
   {
     i=i+j;
     j=i-j;
     i=i-j;
   }
   for (k=i;k>0;k--)
   {
    if (j%k==0 && i%k==0)
      { 
       printf("the common divisor is :%d\n",k);
       break;
      }
   }
   return 0;
}
