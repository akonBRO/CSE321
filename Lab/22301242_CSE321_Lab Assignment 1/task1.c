#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){

FILE *myFile;
char userText[100];  
int stop=0;        

if (argc<2) {
printf("file name.\n");
return 1;}
myFile =fopen(argv[1],"a");
if (myFile==NULL) {
printf("fail\n");
return 1; }
printf("text.\n");
printf("type -1.\n");
while (stop == 0) {
printf("Input:");
fgets(userText, sizeof(userText),stdin);

int length=strlen(userText); 
if(length>0 && userText[length-1]=='\n') {
    userText[length-1]='\0';}
if (strcmp(userText,"-1")==0) {
    stop=1;
} else {
    fputs(userText, myFile);
    fputs("\n", myFile);
}
}

fclose(myFile);
printf("closed.\n");

return 0;
}
