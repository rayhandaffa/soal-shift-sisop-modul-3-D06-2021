#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int id;
int pipes1[2];
int pipes2[2];

int main(int argc, char const *array[]) 
{
    // create pipe1
    if (pipe(pipes1) == -1) 
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Pipe Creation is Failde:(\n");
        exit(EXIT_FAILURE);
    }

  // fork (ps aux)
  //kalo misalkan error
    if ((id = fork()) == -1) 
    {
       printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Fork Creation is Failde:(\n");
        exit(EXIT_FAILURE);
    } 
    
    else if (id == 0) 
    {
        // output to pipe1
        dup2(pipes1[1], 1);

        // close fds
        close(pipes1[0]);
        close(pipes1[1]);

        // exec
        char *jalaninPS[] = {"ps", "aux", NULL};
        execv("/bin/ps", jalaninPS);

        // kalo gajalan
        perror("bad exec ps:(");
        _exit(1);
    }
    // parent

    // create pipe2
    if (pipe(pipes2) == -1) 
    {
       printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Pipe2 is Failde:(\n");
        exit(EXIT_FAILURE);
    }

    // fork (grep root)
    if ((id = fork()) == -1) 
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Fork2 is Failde:(\n");
        exit(EXIT_FAILURE);
    } 

    else if (id == 0) 
    {
        dup2(pipes1[0], 0); // input pipes1
        dup2(pipes2[1], 1);// output pipes2

        // tutup pipesnya (fds)
        close(pipes1[0]);
        close(pipes1[1]);
        close(pipes2[0]);
        close(pipes2[1]);

        // exec sort
        char *sort[] = {"sort", "-nrk", "3.3", NULL};
        execv("/bin/sort", sort) ;

        // kalo gajalan
        perror("bad exec grep root");
        _exit(1) ;
    }
//parentnya  
  // tutup yang ga kepake
    close(pipes1[0]);
    close(pipes1[1]);

    // fork (grep sbin)
    if ((id = fork()) == -1) 
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Grep SBIN is Failed:(\n");
        exit(EXIT_FAILURE);
    } 
    
    else if (id == 0) 
    {
        dup2(pipes2[0], 0); // input from pipe2

        // tutup kalo misalkan output to stdoutnya udah 
        close(pipes2[0]);
        close(pipes2[1]);

        // exec
        char *argv[] = {"head", "-5", NULL};
        execv("/bin/head", argv);

        // exec gajalan
        perror("bad exec grep sbin");
        _exit(1);
    }

}

