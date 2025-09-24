#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
pid_t ff;
pid_t sf;
int ws1; 
int ws2;
ff= fork(); 
if (ff<0){
    return 1;}

if (ff==0){
    sf=fork(); 

    if(sf<0){
        return 1;
 }

    if (sf==0){
        printf("I am grandchild\n");} else {
        wait(&ws1);
        printf("I am child\n");
    }}
    else{
    wait(&ws2);
    printf("I am parent\n");}

return 0;
}
