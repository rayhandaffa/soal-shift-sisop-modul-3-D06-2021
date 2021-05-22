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

void writefile(char dir[]) {
    FILE *file = fopen(dir,"w");
    char buffer[1024]={0};

    memset(buffer,0,sizeof(buffer));
    int len = read(sd,buffer,1024);
    fprintf(file,"%s",buffer);
    
    printf("break\n");
    fclose(file);
}

void Add() { 
    char publisher[1024] = {0};
	char tahun[1024] = {0};
	char path[1024] = {0};
    sends("Publisher:");
    bRead();
    strcpy(publisher,recieve);
    sends("Tahun Publish:");
    bRead();
    strcpy(tahun,recieve);
    sends("Filepath:");
    bRead();
    strcpy(path,recieve);

    char *ptr1;
    char slash ='/';
    ptr1 = strrchr( path, slash );
    ptr1++;
    char fname[100];
    strcpy(fname,ptr1);
    char ext[10];
    char *ptr2;
    char dot ='.';
    ptr2 = strrchr( path, dot );
    ptr2++;
    strcpy(ext,ptr2);
    char dir[300] = "/home/zulfa/Documents/modul3/shift3/FILES/";
    strcat(dir,fname);
    FILE* tsv = fopen("files.tsv","a");
    char info[5000];
    sprintf(info,"%s\t%s\t%s\t%s\t%s\n",fname,publisher,tahun,ext,path);
    fputs(info,tsv);
    fclose(tsv);
    writefile(dir); 
    sends("File berhasil ditambahkan\n");
}

void download () {
    sends("namafile.extension\n");
    bRead();
    bool flag;
    char find[200];
    recieve[strcspn(recieve,"\n")] =0;
    strcpy(find,recieve);
    FILE* file = fopen("files.tsv", "r");
    char lines[1024];
    char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
    while (fgets(lines,1024,file)) {
        // char *token;
        // char found[100];
        // char *linesz = lines;
        // char *rest;
        // token = strtok_r(linesz,"\t",&rest);
        // strcpy(found,token);
        sscanf(lines,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
        if (strcmp(find,filename)==0){
            flag = true;
            break;
        }
    }
    if (flag==true) {
        sends("File ditemukan\n");
        char temp[104]="/home/zulfa/Documents/modul3/shift3/FILES/";
        strcat(temp,find);
        FILE *sfd = fopen(temp,"rb");  
        char data[4096] = {0};

        memset(data,0,4096);
        size_t size = fread(data,sizeof(char),4096,sfd);
        send(sd,data,strlen(data),0);

        // printf("break");
        fclose(sfd);
    }
    else {
        sends("File tidak ditemukan\n");
    }
}

void delete() {
    // sends("namafile.extension\n");
    bRead();
    bool flag = false;
    char finds[1024]={0};
    strcpy(finds,recieve);
    printf("%s\n",finds);
    FILE* fileR = fopen("files.tsv","r");
    FILE* fileW = fopen("temp.tsv","w");
    char data[1024] = {0};
    char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
    while(fgets(data,1024,fileR)!=NULL){
        sscanf(data,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
        if (strcmp(filename,finds)!=0) {
        fprintf(fileW,"%s",data);
        } 
        if (strcmp(filename,finds)==0){
            flag = true;
        }
        bzero(data,1024);
    }
    fclose(fileW);
    fclose(fileR);
    if (flag == true) {
    remove("files.tsv");
    rename("temp.tsv","files.tsv");
    // FILE* log = fopen("running.log","a");
    // fprintf(log,"Hapus: %s %s",finds,upass);
    // fclose(log);
    char oldFile[200]= {0};
    char renamed[200]={0};
    char temp[104]="/home/zulfa/Documents/modul3/shift3/FILES/";
    strcat(oldFile,temp);
    strcat(renamed,temp);
    strcat(oldFile,finds);
    strcat(renamed,"old-");
    strcat(renamed,finds);
//    printf("d");
    rename(oldFile,renamed);
    sends("Delete sukses\n");
    }
    else {
        sends("File tidak ditemukan\n");
    }
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
                    strcpy(sent,"Masukkan ID dan password");
                    send(new_socket,sent,strlen(sent),0);
                    bRead();
                    daftar(recieve);
                    sends("Registrasi berhasil, silahkan login!\n");
                    continue;
                }
                if (strcmp(choice,"login")==0) {
                    printf("login\n");
                    sends("Masukkan ID dan password");
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

                if(loggedIn==true) {
                    if (strcmp(choice,"add")==0) {
                        printf("Add\n");
                        Add(); 
                    }
                    else if(strcmp(choice,"download")==0) {
                        printf("Download\n");
                        download();
                    }
                    else if(strcmp(choice,"delete")==0) {
                        printf("Delete\n");
                        delete();
                    }
                }
            }   
        }
    return 0;
}