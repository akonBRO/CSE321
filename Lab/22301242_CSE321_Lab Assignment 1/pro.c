// mainprog.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
int myArray[100];
int i,j,temp;
int telems;
pid_t cid;
FILE *fp;

if (argc<2) {
    return 1;}
telems=argc- 1;

for (i=1; i<argc;i++) {
    myArray[i-1]=(argv[i]);
}

cid=fork();
if (cid<0) {
    return 1;
}

if (cid== 0) {

    for (i=0;i<telems-1; i++) {
        for (j=0; j<telems-i-1; j++) {
            if (myArray[j]<myArray[j+ 1]) {
                temp=myArray[j];
                myArray[j]=myArray[j+1];
                myArray[j+1]=temp;
        }}
    }

    for (i=0;i<telems;i++) {
        printf("%d",myArray[i]);
    }
    printf("\n");
    fp = fopen("datafile.txt", "w");
    if (fp == NULL) {
        return 1;
    }
    for (i=0; i<telems;i++) {
        fprintf(fp,"%d\n", myArray[i]);
}

    fclose(fp);
} else {
    wait(NULL); 

    fp = fopen("datafile.txt", "r");
    if (fp == NULL) {
        return 1;   }
    int value;
    while (fscanf(fp,"%d",&value)!= EOF) {
        if (value%2== 0) {
            printf("%d is even\n",value); } else {
            printf("%d is odd\n",value);
        } }

    fclose(fp); }

return 0;
}
