#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
extern int *fibArr;
extern int n;
extern int *searchArr;
extern int searchCount;

void *makefib(void *arg);
void *srcfib(void *arg);

int main()
{ pthread_t t1, t2;
    int i;
    int check; 

    printf("Enter the term of fibonacci sequence:\n");
    scanf("%d", &n);
    if(n<0||n>40){
        printf("Invalid input for n, must be 0 to 40 only.\n");
        return 0;}
    pthread_create(&t1, NULL,makefib, NULL);
    pthread_join(t1, NULL);
    for(i=0;i<=n;i=i+1) {
        printf("a[%d]=%d\n",i,fibArr[i]); }
    printf("How many numbers you are willing to search?:\n");
    scanf("%d",&searchCount);
    if(searchCount<= 0)
    {
        printf("Search count must be positive.\n");
        return 0; }
    searchArr=(int*) malloc(searchCount * sizeof(int));
    if(searchArr==NULL)
 {      printf("Memory allocation failed for search array.\n");
        return 0; }

    for(i=0;i<searchCount;i=i+1){
        printf("Enter search %d:\n", i+1);
        scanf("%d",&searchArr[i]);}

    pthread_create(&t2,NULL,srcfib,NULL);
    pthread_join(t2,NULL);
    free(fibArr);
    free(searchArr);
    return 0;
}
