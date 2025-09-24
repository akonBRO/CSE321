#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int *fibArr;
int n;

void *makefib(void *arg)
{   int i,j;
    j=0;  
    fibArr=(int*)malloc((n+1)*sizeof(int));
    if(fibArr == NULL)
    {
        printf("error!\n");
        pthread_exit(NULL); }
    if(n==0)
{
        fibArr[0] = 0;   }
    else if(n==1)
    {
        fibArr[0]=0;
        fibArr[1]=1;  }
    else  {
      fibArr[0]=0;
        fibArr[1] =1;
        for(i=2;i<=n;i=i+1)
  {
            int temp1,temp2;
            temp1=fibArr[i-1];
            temp2=fibArr[i-2];
            fibArr[i] =temp1+temp2; }}

    pthread_exit(NULL);
}
