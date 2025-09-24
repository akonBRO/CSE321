#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern int *fibArr;
extern int n;
int *srcarr;
int srccnt;

void *srcfib(void *arg)
{ int i,index,ans;
    for(i=0;i<srccnt;i=i+1)
    {
        index=srcarr[i];
        if(index>=0)
        {   if(index<=n)
            {
            ans=fibArr[index];
            printf("result of search #%d= %d\n",i+1,ans); }
            else
     {
              ans=-1;
                printf("result of search #%d= %d\n",i+1,ans); }
        }
        else {
            printf("result of search #%d= -1\n",i+1); }
    }
    pthread_exit(NULL);
}
