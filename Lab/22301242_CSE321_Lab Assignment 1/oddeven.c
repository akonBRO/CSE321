#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){

int numbers[100];
int i;
int num;

if (argc<2) {
    return 1;}
for (i=1; i<argc; i++) {
    num= (argv[i]);
    numbers[i-1]= num;}

for (i=0; i<argc-1; i++) {
    if (numbers[i]%2==0) {
        printf("%d is even\n",numbers[i]);
} else {
        printf("%d is odd\n",numbers[i]);}
}

return 0;
}
