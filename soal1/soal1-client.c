#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#define PORT 8080

char sent[1024];
char recieve[1024];
char buff[1024];
bool loggedIn=false;
int sock;

void resR() {
    read(sock,recieve,1024);
    printf("%s\n",recieve);
    memset(recieve,0,sizeof(recieve));
}

void sends(char data[]) {
    send(sock,data,strlen(data),0);
    memset(sent,0,sizeof(sent));
}

void Reg() {
    char id[50];
    char pwd[50];
    resR();
    scanf("%s",id);
    scanf("%s", pwd);
    
    sprintf(sent,"%s:%s\n",id,pwd);
    sends(sent);
    // send(sock,sent,strlen(sent),0);
    // memset(sent,0,sizeof(sent));
    resR();
}


void Log() {
    resR();
    char id[50];
    char pwd[50];
    scanf("%s",id);
    scanf("%s", pwd);
    sprintf(sent,"%s:%s\n",id,pwd);
    sends(sent);
    // send(sock,sent,strlen(sent),0);
    // memset(sent,0,sizeof(sent));
    read(sock,recieve,1024);
    printf("%s\n",recieve);
    if(recieve[0]=='L'){
        loggedIn=true;
        printf("[1]add\n[2]download\n[3]delete\n[4]see\n[5]find\n");
    }
    memset(recieve,0,sizeof(recieve));
}

void Add()
{
    char temp[1024];
    for (int i=0;i<3;i++) {
        resR();
        scanf("%s",temp);
        temp[strcspn(temp,"\n")] =0;
        sends(temp);
    }
    FILE *sfd = fopen(temp,"rb");  
    char data[1024] = {0};
    
    memset(data,0,1024);
    size_t size = fread(data,sizeof(char),1024,sfd);
    send(sock,data,1024,0);
    
    fclose(sfd);
    resR();
}

void download() {
    resR();
    char temp[1024];
    scanf("%s",temp);
    temp[strcspn(temp,"\n")] =0;
    sends(temp);
    read(sock,recieve,1024);
    printf("%s\n",recieve);
    if (recieve[5]=='d') {
        char dir[300] = "/home/zulfa/Documents/modul3/shift3/";
        strcat(dir,temp);
        FILE *file = fopen(dir,"w");
        char buffer[4096]={0};
       
            memset(buffer,0,sizeof(buffer));
            int len = read(sock,buffer,4096); 
            fprintf(file,"%s",buffer);
        fclose(file);
    }
}

void delete() {
    // resR();
    char temp[1024];
    scanf("%s",temp);
    temp[strcspn(temp,"\n")]=0;
    sends(temp);
    resR();
}

void see(){
    char bigbuff[10000];
    read (sock,bigbuff,10000);
    printf("%s\n",bigbuff);
    memset(bigbuff,0,sizeof(bigbuff));  
}

void find(){
    printf("Masukkan nama file\n");
    char find[200] = {0};
    scanf("%s",find);
    find[strcspn(find,"\n")]=0;
    sends(find);
    resR();
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    else {
        read(sock,recieve,1024);
        printf("%s\n",recieve);
        memset(recieve,0,sizeof(recieve));
    }

    while(1)
    {
        pthread_t thread;
        char choice[50];

        scanf("%s",choice);
        strcpy(sent,choice);
        send(sock,sent,strlen(sent),0);
        memset(sent,0,sizeof(sent));
       if (strcmp(choice,"register")==0){
            Reg();
            continue;
        }
        else if (strcmp(choice,"login")==0){
            Log();
        }
          
        else if (strcmp(choice,"add")==0 && loggedIn) {
            Add();
        }
        else if (strcmp(choice,"download")==0 && loggedIn) {
            download();
        }
        else if (strcmp(choice,"delete")==0 && loggedIn) {
            delete();
        }
        else if (strcmp(choice,"see")==0 && loggedIn) {
            see();
        }
        else if (strcmp(choice,"find")==0 && loggedIn) {
            find();
        }
        else {
            printf("Command salah,perhatikan penulisan anda\n");
            continue;
        }
    }
    return 0;
}
