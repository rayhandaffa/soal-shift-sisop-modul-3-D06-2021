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
int soc;

void Reg() {
    read(soc,recieve,1024);
    printf("%s\n",recieve);
    memset(recieve,0,sizeof(recieve));
    char id[50];
    char pwd[50];
    scanf("%s",id);
    scanf("%s", pwd);
    sprintf(sent,"%s:%s\n",id,pwd);
    send(soc,sent,strlen(sent),0);
    memset(sent,0,sizeof(sent));
    read(soc,recieve,1024);
    printf("%s\n",recieve);
    memset(recieve,0,sizeof(recieve));
}


void Log() {
    read(soc,recieve,1024);
    printf("%s\n",recieve);
    memset(recieve,0,sizeof(recieve));
    char id[50];
    char pwd[50];
    scanf("%s",id);
    scanf("%s", pwd);
    sprintf(sent,"%s:%s\n",id,pwd);
    send(soc,sent,strlen(sent),0);
    memset(sent,0,sizeof(sent));
    read(soc,recieve,1024);
    printf("%s\n",recieve);
    if(recieve[0]=='L'){
        loggedIn=true;
    }
    memset(recieve,0,sizeof(recieve));
}

void resR() {
    read(soc,recieve,1024);
    printf("%s\n",recieve);
    memset(recieve,0,sizeof(recieve));
}

void sends(char data[]) {
    send(soc,data,strlen(data),0);
    memset(sent,0,sizeof(sent));
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
  
    if (connect(soc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    else {
        read(soc,recieve,1024);
        printf("%s\n",recieve);
        memset(recieve,0,sizeof(recieve));
    }

    while(1)
    {
        pthread_t thread;
        char choice[50];

        scanf("%s",choice);
        strcpy(sent,choice);
        send(soc,sent,strlen(sent),0);
        memset(sent,0,sizeof(sent));
       if (strcmp(choice,"register")==0){
            Reg();
            continue;
        }
        else if (strcmp(choice,"login")==0){
            Log();
        }
        else {
            printf("Command salah,perhatikan penulisan anda\n");
            continue;
        }
    }
    return 0;
}