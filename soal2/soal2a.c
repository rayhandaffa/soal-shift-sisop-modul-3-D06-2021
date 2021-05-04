#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread

//matriks asal
int matriks1[4][3] =
{
    1,2,3,
    4,5,6,
    7,8,9,
    10,11,12
};

int matriks2[3][6] =
{
    13,14,15,16,17,18,
    19,20,1,2,3,4,
    5,6,7,8,9,10
};

//ukuran matriks
int mat1I = 4, 
    mat1J = 3;
int mat2I = 3,
    mat2J = 6;

//nilai yang di return inisialisasi awal
int returnNilai[4][6];

//semaphore buat return matriks nya
int returnMatriks = 0;

void *ngitungMatriks(void *simp)
{
    int *pesan;
    pesan = (int *)simp;

    int iMatriks1 = pesan[0];
    int jMatriks2 = pesan[1];
    free(pesan);

    int jumlah = 0;
    int i;

    for(i = 0; i < mat1J; i++)
    {
        jumlah += matriks1[iMatriks1][i]*matriks2[i][jMatriks2];
    }

    returnNilai[iMatriks1][jMatriks2] = jumlah;
}

void *printHasil(void *pointer)
{
    char *pesan;
    pesan = (char *)pointer;

    printf("%s \n", pesan);
    for(int i = 0; i <= 7; i++)
    {
        printf("%s %d \n", pesan, i);
    }
}

int main()
{
    pthread_t thread[mat1I][mat2J]; //inisialisasi awal
    int matriks[mat1J][mat2J];

    //mengirimkan single thread untuk setiap elemen dalam matriks hasil
    for(int i = 0; i < mat1I; i++)
    {
        for(int j = 0 ; j < mat2J; j++)
        {
            int *pesan;
            pesan = malloc(sizeof(int) * 2);
            pesan[0] = i;
            pesan[1] = j;
            void *pesanPtr = (void *)pesan;
            matriks[i][j] = pthread_create(&thread[i][j], NULL, ngitungMatriks, pesanPtr); //buat thread pertama
            //kalo error
            if(matriks[i][j])
            {
                fprintf(stderr, "Error - pthread_create() return code: %d\n", matriks[i][j]);
                exit(EXIT_FAILURE);
            }
         // printf("pthread_create() for thread 1 returns: %d\n", iret[i][j]);
        }
    }

    //join mat1I dan mat2J
    for(int i = 0; i < mat1I; i++)
    {
        for (int j = 0; j < mat2J; j++)
        {
           pthread_join( thread[i][j], NULL );
        //    printf("thread joined\n");
        }
    }

    //returnnilai
    for(int i = 0; i < mat1I; i++)
    {
        printf("%d ", returnNilai[i][0]);
        for(int j = 1; j < mat2J; j++)
        {
            printf("%d ", returnNilai[i][j]);
        }
        printf("\n");
    }
    exit(EXIT_SUCCESS);
}