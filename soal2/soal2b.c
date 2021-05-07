#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

//from output 2a 
int baris = 4, kolom = 6;
key_t keyshm = 1234;

void nutupPipe(int pipes[])
{
    //nutup pipe kalo misalkan ga digunain lagi
    //buat read pipenya
    close(pipes[0]);

    //buat write pipenya
    close(pipes[1]);
}

void initSharedmemory()
{

}

void jalaninProgram()
{
    int pipeNih[2]; //kan kanan kiri jadinya 2 giruch
    //buat pipenya
    int childpid = fork();
    //buat fungsi kalo misalkan gagal buat pipenya duluch
    if(pipe(pipeNih) == -1)
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Child Creation is Failde:(\n");
        exit(EXIT_FAILURE);
    }

    //pas di child nya
    if(childpid == 0)
    {
        //link stdout ke pipe inputnyA
        //ARTINYA TUH : si pipe INPUT nya dijadiin STDOUT duluch
        dup2(pipeNih[1], STDOUT_FILENO);

        nutupPipe(pipeNih);

        //jalanin programnyach
        char *jalanin[] = {"soal2a", NULL};
        execv("./soal2a.exe", jalanin); 
    }

    //pas di parentnya
    else
    {
        wait(NULL); //nungguin tuh ya sampe finish itu jalanin nya
        //shared memory
        int idShm = shmget(keyshm, sizeof(int)*20, IPC_CREAT | 0666);
        int *buffer = (int *)shmat(idShm, NULL, 0);

        //link stdin ke pipe OUTPUTnyA
        //ARTINYA TUH : si pipe OUTPUT nya dijadiin STDIN
        dup2(pipeNih[0], STDIN_FILENO);

        //get input
        int masuk; 
        for(int a = 1; a <= 20; a++)
        {
            scanf("%d", &masuk);
            buffer[a] = masuk;
            // printf("masuk = %d\n", masuk);
            // printf("buffer = %d\n", buffer[a]);
        }

        for(int a = 1; a <= 4; a++)
        {
            for(int b = 1; b <= 6; b++)
            {
                printf("%d ", buffer[a*4+b]);
            }
            printf("\n");
        }

        //lepaskan dari shared memory bund
        shmdt(buffer);

    }
}
int main(int argc, char const *argv)
{
    //buat child 
    pid_t childpid = fork();

    //kalo misal childnya error 
    if(childpid < 0)
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Child Creation is Failde:(\n");
        exit(EXIT_FAILURE);
    }

    //pas di child 
    if(childpid == 0)
    {
        
        // jalanin programnyach
        // char *jalanin[] = {"soal2a", NULL};
        // execv("./soal2a.exe", jalanin); 
        jalaninProgram();
        exit(EXIT_SUCCESS);
    }

    else
    {
        wait(NULL); //ini buat bakalan nungguin sampe program 2a nya selese 

        //dapetin ouputnya dari program sebelum yang shared memory
        int idShm = shmget(keyshm, sizeof(int)*20, IPC_CREAT | 0666);
        int (*buffer)[5];
        buffer = (int (*)[5])shmat(idShm, NULL,0);

        //dapetin input dari program terakhir
        int masuk; 
        for(int a = 1; a <= 4; a++)
        {
            printf("%d", buffer[a][0]);
            for(int b = 1; b <= 6; b++)
            {
                printf("%d ", buffer[a][b]);
            }
            printf("\n");
        }
        shmdt(buffer);
        shmctl(idShm, IPC_RMID, NULL);

    }
    return 0;
}