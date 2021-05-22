#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <dirent.h>

#define PORT 8080

char sent[1024];
char recieve[1024];
char buffer[1024];
char user[30];
char upass[100];
char choice[50];
bool connected = false;
int server_fd , new_socket , client_socket[30]= {0},max_clients = 30 , activity,max_sd, i , valread , sd; // GFG
fd_set readfds;
bool loggedIn = false;

void daftar(char str[])
{
    printf("DAFTAR USER\n");
    char idpwd [100];
    strcpy(idpwd,str);
    printf("%s\n",idpwd);
    FILE* file = fopen("akun.txt", "a") ;
    fputs(idpwd,file);
    fclose(file);
}

void resR() {
    memset(recieve,0,sizeof(recieve));
}

void bRead() { // check Disconnects + Read vals
    resR();
    int check;
    if ((check = read(sd,recieve,1024)) == 0){
        connected=false;
        loggedIn=false;
        close(sd);
        for(i=0;i<30;i++)
        {
            client_socket[i]=client_socket[i+1];
        }
        client_socket[29]=0;
    }
}

void bReadCommand() { // check Disconnects + Read vals
    resR();
    int check;
    if ((check = read(sd,choice,100)) == 0){
        connected=false;
        loggedIn=false;
        close(sd);;
        for(i=0;i<30;i++)
        {
            client_socket[i]=client_socket[i+1];
        }
        client_socket[29]=0;
    }
}

bool LogUser(char str[]) {
    printf("User Login\n");
    char idpwd [100];
    strcpy(idpwd,str);
    printf("%s\n",idpwd);
    char *id;
    char tok[2]=":";
    char find[100];
    FILE* file = fopen("akun.txt","r");
    while (fgets(find,100,file)) {
        // printf("%s%s\n",find,idpwd);
        if (strcmp(find,idpwd)==0) {
            strcpy(upass,idpwd);
            fclose(file);
            id = strtok(idpwd,tok);
            strcpy(user,id);
            loggedIn = true;
            return true;
        }
    }
    return false;
}

void sends(char data[]) {
    send(sd,data,strlen(data),0);
    memset(sent,0,sizeof(sent));
}

int main(int argc, char const *argv[]) {  
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    DIR* dir = opendir("FILES");
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        mkdir("FILES", 0777);
    }
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) <0 ) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1) {
        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(server_fd, &readfds);  
        max_sd = server_fd;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        } 
         //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(server_fd, &readfds))  
        {  
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }
             for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }
        sd = client_socket[0];
        connected = true;
        printf("CONNECTED\n");
        strcpy(sent,"[1] register\n[2] login");
        printf("%s\n",sent);
        send(new_socket,sent,strlen(sent),0);
        memset(sent,0,sizeof(sent));
        while(connected) {
            pthread_t thread ;
            memset(choice,0,sizeof(choice));
            bReadCommand();
            printf("Command: %s\n",choice);
            //int a;
            //scanf("%d",&a);
            if (loggedIn==false)
            {
                if (strcmp(choice,"register")==0)
                {
                    printf("register\n");
                    strcpy(sent,"Register\nMasukkan ID dan password");
                    send(new_socket,sent,strlen(sent),0);
                    bRead();
                    daftar(recieve);
                    sends("Registrasi berhasil, silahkan login!\n");
                    continue;
                }
                if (strcmp(choice,"login")==0) {
                    printf("login\n");
                    sends("Login\nInput ID dan password");
                    bRead();
                    if (LogUser(recieve)) {
                        sprintf(sent,"Login sukses!\n");
                        sends(sent);
                    }
                    else {
                        sends("ID atau password salah!\n");
                    }
                        memset(recieve,0,sizeof(recieve));
                        continue;
                    }
                }
            }   
        }
    return 0;
}