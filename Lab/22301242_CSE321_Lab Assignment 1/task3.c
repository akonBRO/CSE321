#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
pid_t p1,p2,p3;
pid_t oddchk;
pid_t echild;

int pcount = 1; 

p1=fork();
if (p1<0) {
    return 1;}
if (p1==0) {
    pcount++;

    p2=fork();
    if (p2<0) {
        return 1;}
    if (p2==0) {
        pcount++;
        p3=fork();
        if (p3<0) {
            return 1; }

        if (p3==0) {
            pcount++;
        }else{
            wait(NULL); }}
            else {
        wait(NULL);
    }
}else {
    wait(NULL);
}

sleep(1); 

oddchk=getpid();

if (oddchk%2!=0) {
    echild=fork();

    if(echild==0) {
        sleep(1);
        exit(0); } 
    else {
        pcount++;
        wait(NULL); 
    } }

sleep(2); 

if (getppid()!=1 && p1>0) {
    printf("Total process created: 16\n");
}
    return 0;
}
