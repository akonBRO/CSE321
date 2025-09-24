#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define tot 10
#define chr 3
pthread_mutex_t mtx;
sem_t semstu;  
sem_t semst;   
int w=0;      
int d=0;    

void *tjob(void *a){
    while (d<tot) {
    sem_wait(&semstu);
    pthread_mutex_lock(&mtx);
        if (w>0){
            w--;
            printf("a waiting student started getting consultation\n");
            printf("num of students now waiting: %d\n", w); }
        pthread_mutex_unlock(&mtx);
        sem_post(&semst);
        printf("st giving consultation\n");
        sleep(1); }
    return 0;
}
void *sjob(void *a){
    int id=*(int*)a;
    sleep(rand()%3);
    pthread_mutex_lock(&mtx);
    if (w<chr) {
        w++;
        printf("student %d started waiting for consultation\n",id);
        pthread_mutex_unlock(&mtx);
        sem_post(&semstu);
        sem_wait(&semst); 
        printf("student %d is getting consultation\n",id);
        sleep(1);
        printf("student %d finished getting consultation and left\n",id);
        pthread_mutex_lock(&mtx);
        d++;
        printf("num of served students: %d\n", d);
        pthread_mutex_unlock(&mtx);
} else{
        printf("no chairs left. student %d leaving...\n",id);
        pthread_mutex_unlock(&mtx);
        sleep(1);
        printf("student %d finished getting consultation and left\n",id);
        pthread_mutex_lock(&mtx);
        d++;
        printf("num of served students: %d\n",d);
        pthread_mutex_unlock(&mtx);}
    return 0;}

int run(){
    pthread_t t;
    pthread_t s[tot];
    int id[tot];
    int i;
    pthread_mutex_init(&mtx,0);
    sem_init(&semstu,0,0);
    sem_init(&semst,0,0);
    pthread_create(&t,0,tjob,0);
    for(i=0;i<tot;i++){
        id[i]=i;
        pthread_create(&s[i],0,sjob,&id[i]);}
    for(i=0;i<tot;i++){
        pthread_join(s[i],0);}
    pthread_join(t,0);
    pthread_mutex_destroy(&mtx);
    sem_destroy(&semstu);
    sem_destroy(&semst);
    return 0;
}
