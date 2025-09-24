#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
pid_t cpid;
pid_t gpid1, gpid2, gpid3;

printf("1.pid:0\n");
cpid=fork();

if (cpid<0) {
    return 1;
}
if (cpid==0) {
    printf("2.cid: %d\n", getpid());
gpid1=fork();
    if (gpid1<0) {
        exit(1); }
if (gpid1==0) {
        printf("3.gcid: %d\n", getpid());
        exit(0); }
    gpid2=fork();

    if (gpid2<0) {
        exit(1); }
    if (gpid2==0) {
        printf("4.gcid: %d\n",getpid());
        exit(0);}
    gpid3=fork();
    if (gpid3< 0) {
        exit(1);
}

    if (gpid3==0) {
        printf("5.gcid: %d\n",getpid());
        exit(0); }
    wait(NULL);
    wait(NULL);
    wait(NULL);

    exit(0);
} else {
    wait(NULL); }

return 0;
}
