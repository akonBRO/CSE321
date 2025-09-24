
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


struct mydata{
    char sel[100];  
    int balance;      
};

int main(){
    int shmid;
    struct mydata *ptr;
    int p[2];
    pid_t cpid;

    shmid=shmget(1234,sizeof(struct mydata),0666 | IPC_CREAT);
    if (shmid<0) {
        printf("error making shared memory\n");
        return 0; }
    ptr=(struct mydata*) shmat(shmid, NULL, 0);
    if (ptr==(void*)-1) {
        printf("error attaching shared memory\n");
        return 0; }

    if (pipe(p) == -1) {
        printf("pipe not created\n");
        return 0;}
    printf("Provide Your Input From Given Options:\n");
    printf("1. Type a to Add Money\n");
    printf("2. Type w to Withdraw Money\n");
    printf("3. Type c to Check balance\n");

    char inp[100];
    scanf("%s",inp);

    strcpy(ptr->sel,inp); 
    ptr->balance=1000;       
    printf("Your selection: %s\n", ptr->sel);
    cpid=fork();
    if (cpid<0) {
        printf("fork error\n");
        return 0;}

    if (cpid==0){
        if (strcmp(ptr->sel,"a")==0) {
            int x;
            printf("Enter amount to be added: ");
            scanf("%d",&x);
            if (x>0) {
                ptr->balance=ptr->balance+x;
                printf("balance added successfully\n");
                printf("Updated balance after addition: %d\n", ptr->balance);
} else {
                printf("Adding failed, Invalid amount\n");}}
        else if (strcmp(ptr->sel, "w")==0) {
            int y;
            printf("Enter amount to be withdrawn: ");
            scanf("%d",&y);
            if (y > 0 && y < ptr->balance) {
                ptr->balance = ptr->balance - y;
                printf("balance withdrawn successfully\n");
                printf("Updated balance after withdrawal: %d\n",ptr->balance);
         } else{
                printf("Withdrawal failed,Invalid amount\n"); }
 }
       else if (strcmp(ptr->sel,"c")==0) {
            printf("Your current balance is: %d\n",ptr->balance); }
        else{
            printf("Invalid selection\n");
  }

        char text[]="Thank you for using";
        write(p[1],text,strlen(text)+1);
        close(p[1]);

        exit(0);   }
    else {
        wait(NULL);
        char buff[100];
        read(p[0], buff, sizeof(buff));
        printf("%s\n", buff);
        close(p[0]);
        shmdt(ptr);
        shmctl(shmid, IPC_RMID, NULL);    }

    return 0;
}



