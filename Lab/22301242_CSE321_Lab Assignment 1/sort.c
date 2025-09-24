#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
int i, j;
int numbers[100];
int temp;
int size;

if (argc<2) {
    return 1;}

size=argc- 1;
for (i= 1; i<argc; i++) {
    numbers[i-1]= (argv[i]);
}

for (i=0;i<size; i++) {
    for (j =0; j<size- 1; j++) {
        if (numbers[j]< numbers[j+1]) {
            temp=numbers[j];
            numbers[j]= numbers[j+1];
            numbers[j+1]=temp; }
}
}

printf("Sorted numbers in descending order:\n");
for (i=0; i<size; i++) {
    printf("%d ", numbers[i]);
}
printf("\n");

return 0;
}
