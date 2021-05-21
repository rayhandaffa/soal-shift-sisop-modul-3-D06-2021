#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
    int xresult[24];
    int inputs[24];
    int i,j,iter,add=0;

    pthread_t tid[24];

struct pair{
    int angka;
    int batas;
};

void* faktorial(void *pairs){

    struct pair *pair = (struct pair *)pairs;
    int angka = pair->angka;
    int batas = pair->batas;
    if(angka == 0 || batas == 0){
        printf("0\n");
    }else{
        if(batas > angka){
            while(angka>0){
                if(angka == 1){
                    printf("%d",angka);
                }else{
                    printf("%d*",angka);
                }
                angka--;
            }
            printf("\n");
        }else if(angka >= batas){
            while(batas>0){
                if(batas == 1){
                    printf("%d",angka);
                }else{
                    printf("%d*",angka);
                }
                angka--;
                batas--;
            }
            printf("\n");
        }
    }
}

void main(){

    printf("Masukkan 24 Input :\n");
    for(iter=0;iter<24;iter++){
        scanf("%d",&inputs[iter]);
    }

    puts("");
    
        int a=0;
        key_t key = 1234;
        int *value;
        //membuat shared-memory segmen
        int shmid = shmget(key, sizeof(int)*6*4, IPC_CREAT | 0666);
        //menautkan pointer ke segmen memory pertama
        value = shmat(shmid, NULL, 0);
     for(iter=0;iter<24;iter++){
            xresult[iter] = value[iter];
            struct pair pair;
            pair.angka = xresult[iter];
            pair.batas = inputs[iter];

            int error = pthread_create(&tid[iter],NULL,faktorial,(void*)&pair);

            if(error != 0){
                printf("\nCan't create thread : [%s]",strerror(error));
            }   

            pthread_join(tid[i],NULL);
    }

    sleep(50);

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}