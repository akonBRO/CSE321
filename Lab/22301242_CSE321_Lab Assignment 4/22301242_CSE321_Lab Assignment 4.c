#include <stdio.h>
#include <string.h>
#define MAX_USERS 7
#define MAX_RESOURCES 5
#define MAX_NAME 20

typedef enum{R=1,W=2,X=4}Perm;
typedef struct{char n[MAX_NAME];}Usr;
typedef struct{char n[MAX_NAME];}Res;

typedef struct{char u[MAX_NAME]; int p;}ACLEn;
typedef struct{Res r; ACLEn e[10]; int c;}ACLRes;

typedef struct{char rn[MAX_NAME]; int p;}Cap;
typedef struct{Usr u; Cap c[10]; int cn;}CapUsr;

void prtPerm(int x){
    if(x & R) printf("r ");
    if(x & W) printf("w ");
    if(x & X) printf("x ");}

int gotPerm(int a,int b){
    return (a&b)==b;}

void chkACL(ACLRes *r,const char *u,int p){
    int f=0;
for(int i=0;i<r->c;i++){
        if(strcmp(r->e[i].u,u)==0){
            f=1;
            if(gotPerm(r->e[i].p,p)){
                printf("[ACL] %s CAN ",u); prtPerm(p); printf("on %s : YES\n",r->r.n);
            }else{
                printf("[ACL] %s WANT ",u); prtPerm(p); printf("on %s : NO\n",r->r.n);}
            break;
 }
 }
    if(!f)printf("[ACL] %s NO ENTRY on %s : DENIED\n",u,r->r.n);
}

void chkCap(CapUsr *u,const char *r,int p){
    int f=0;
for(int i=0;i<u->cn;i++){
        if(strcmp(u->c[i].rn,r)==0){
            f=1;
            if(gotPerm(u->c[i].p,p)){
                printf("[CAP] %s CAN ",u->u.n); prtPerm(p); printf("on %s : YES\n",r);
        }else{
                printf("[CAP] %s WANT ",u->u.n); prtPerm(p); printf("on %s : NO\n",r);}
            break;
  }
    }
    if(!f)printf("[CAP] %s NO CAP for %s : DENIED\n",u->u.n,r);}

int main(){
    Usr us[MAX_USERS]={{"Alice"},{"Bob"},{"Charlie"},{"Dave"},{"Eve"},{"Frank"},{"Grace"}};
    Res rs[MAX_RESOURCES]={{"File1"},{"File2"},{"File3"},{"File4"},{"File5"}};
    ACLRes a1={rs[0],{{"Alice",R|W},{"Bob",R},{"Dave",W}},3};
    ACLRes a2={rs[1],{{"Charlie",W|X},{"Eve",R|X}},2};
    ACLRes a3={rs[2],{{"Alice",R|X},{"Frank",W|X}},2};
    ACLRes a4={rs[3],{{"Bob",W|X},{"Grace",R|W}},2};
    ACLRes a5={rs[4],{{"Eve",R|W|X}},1};
    CapUsr c1={us[0],{{"File1",R|W},{"File3",R|X}},2};
    CapUsr c2={us[1],{{"File1",R},{"File4",X}},2};
    CapUsr c3={us[2],{{"File2",W|X}},1};
    CapUsr c4={us[3],{{"File3",W}},1};
    CapUsr c5={us[4],{{"File2",R},{"File5",R|W|X}},2};
    CapUsr c6={us[5],{{"File3",X},{"File4",W}},2};
    CapUsr c7={us[6],{{"File1",R|X},{"File5",W}},2};
    printf("\n--- ACL TESTS ---\n\n");
    chkACL(&a1,"Alice",R);
    chkACL(&a1,"Bob",W);
    chkACL(&a1,"Charlie",R);
    chkACL(&a2,"Charlie",X);
    chkACL(&a3,"Dave",W);
    chkACL(&a4,"Grace",W);
    chkACL(&a5,"Eve",X);
    printf("\n--- CAP TESTS ---\n\n");
    chkCap(&c1,"File1",W);
    chkCap(&c1,"File3",X);
    chkCap(&c2,"File4",X);
    chkCap(&c3,"File2",R);
    chkCap(&c4,"File3",W);
    chkCap(&c5,"File5",X);
    chkCap(&c6,"File4",W);
    chkCap(&c7,"File5",X);
    printf("\n*** END ***\n");
    return 0;
}
