
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct msg{
    long int type;
    char txt[6];};

int main(){
    int qid;
    struct msg m;
    pid_t pid1,pid2;

    qid=msgget(1234,0666 | IPC_CREAT);
    if (qid<0){
        printf("queue error\n");
        return 0;}
    char name[20];
    printf("Please enter the workspace name:\n");
    scanf("%s",name);
    if (strcmp(name,"cse321")!=0) {
        printf("Invalid workspace name\n");
        msgctl(qid,IPC_RMID,NULL);
        return 0;}
    strcpy(m.txt, name);
    m.type=1;
    msgsnd(qid, &m,sizeof(m.txt),0);
    printf("Workspace name sent to otp generator from log in: %s\n", name);
    pid1=fork();
    if (pid1==0){
        struct msg m2;
        msgrcv(qid, &m2, sizeof(m2.txt),1,0);
        printf("OTP generator received workspace name from log in: %s\n", m2.txt);
        int otp=getpid();
        sprintf(m2.txt,"%d",otp);
        m2.type=2;
        msgsnd(qid,&m2,sizeof(m2.txt),0);
        printf("OTP sent to log in from OTP generator: %s\n", m2.txt);
        m2.type=3;
        msgsnd(qid, &m2, sizeof(m2.txt), 0);
        printf("OTP sent to mail from OTP generator: %s\n", m2.txt);
        pid2=fork();
        if (pid2==0){
            struct msg m3;
            msgrcv(qid,&m3,sizeof(m3.txt),3,0);
            printf("Mail received OTP from OTP generator: %s\n", m3.txt);
            m3.type=4;
            msgsnd(qid,&m3,sizeof(m3.txt),0);
            printf("OTP sent to log in from mail: %s\n",m3.txt);
            exit(0); }
            else{
            wait(NULL);
            exit(0);  }  }
    else{
        wait(NULL);
        struct msg r1, r2;
        msgrcv(qid,&r1,sizeof(r1.txt),2,0);
        printf("Log in received OTP from OTP generator: %s\n", r1.txt);
        msgrcv(qid, &r2,sizeof(r2.txt),4,0);
        printf("Log in received OTP from mail: %s\n", r2.txt);
        if (strcmp(r1.txt, r2.txt)==0){
            printf("OTP Verified\n");   } 
            else{
            printf("OTP Incorrect\n");        }
        msgctl(qid, IPC_RMID, NULL);}

    return 0;
}
