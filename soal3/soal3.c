#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

// Function to get the file names
char *checkName(char *dir)
{
    char *name = strrchr(dir, '/'); // Finds the last occurrence of the character "/" in a string

    if (name == dir)
        return "";

    return name + 1;
}

// Function to change file extension to lowercase
char *lowercase(char *str)
{
    unsigned char *temp = (unsigned char *)str; // temp points to the file extension

    while (*temp)
    {
        *temp = tolower(*temp); // converts string to lower case
        temp++;
    }

    return str;
}

// Function to get file extension
char *checkExtension(char *dir)
{
    char *unknown = {"Unknown"};
    char *hidden = {"Hidden"};
    char *temp = strrchr(dir, '/');
    char extension[400];

    if (temp[1] == '.')
        return hidden; // hidden file

    int i = 0;
    while (i < strlen(temp) && temp[i] != '.')
        i++; // checks for a 'dot' in the file name

    if (i == strlen(temp))
        return unknown; // unknown file

    for (int j = i; i < strlen(temp); i++)
        extension[i - j] = temp[i]; // get the file extension

    return lowercase(extension + 1); // change file extension to lowercase
}

void *categorize(void *arg)
{
    char *source = (char *)arg;

    char sourcePath[150];
    memcpy(sourcePath, (char *)arg, 400); // take the file path

    char *ptrExtension = checkExt(source); // pointer to the file extension

    char extension[400];
    strcpy(extension, ptrExtension); // copy from pointer to extension

    DIR *dir = opendir(ptrExtension); // try to open the directory with the extension name
    if (dir)
        closedir(dir);
    else if (ENOENT == errno)
        mkdir(ptrExtension, 0755); // if directory does not exist, create a new directory with an extension name

    char *ptrFileName = checkName(sourcePath); // get the file name
    char *current = getenv("PWD");             // get the path where the program is run

    char destPath[402];
    sprintf(destPath, "%s/%s/%s", current, extension, ptrFileName); // concatenate the new path of the file

    rename(sourcePath, destPath); // rename old path to new path
}

void categorizeFolder(char *folderPath, int threadSize)
{
    DIR *ptrFolderPath = opendir(folderPath);
    struct dirent *dirPath;
    pthread_t tid[threadSize]; // initialize thread
    int count = 0;
    char fileName[400][400];

    while ((dirPath = readdir(ptrFolderPath)) != NULL)
    {
        if (dirPath->d_type == DT_REG)
        // if it's a regular file
        {
            sprintf(fileName[count], "%s/%s", folderPath, dirPath->d_name); // file path in the folder

            pthread_create(&tid[count], NULL, categorize, (void *)fileName[count]); // create thread to categorize files

            count++;
        }
        else if ((dirPath->d_type == DT_DIR) &&
                 strcmp(dirPath->d_name, ".") != 0 &&
                 strcmp(dirPath->d_name, "..") != 0)
        // if it's a directory
        {
            char folderPath2[400];
            sprintf(folderPath2, "%s/%s", folderPath, dirPath->d_name); // folder path within the folder
            DIR *ptrFolderPath2 = opendir(folderPath2);                 // open folder
            struct dirent *dirPath2;
            int threadSize2 = 0;

            while ((dirPath2 = readdir(ptrFolderPath2)) != NULL)
                if (dirPath2->d_type == DT_REG) // check if it's regular file
                    threadSize2++;              // adjust thread size

            categorizeFolder(folderPath2, threadSize2); // recursion for folders within folders

            closedir(ptrFolderPath2);
        }
    }

    // as wait, join thread to wait for terminated
    for (int i = 0; i < threadSize; i++)
        pthread_join(tid[i], NULL); // join thread

    closedir(ptrFolderPath);
}

int main(int argc, char *argv[])
{
    if (argc == 0)
    {
        exit(EXIT_FAILURE);
    }

    // soal3a
    if (argc > 2 && strcmp(argv[1], "-f") == 0)
    {
        pthread_t tid[argc - 2]; // initialize thread
        int count = 0;

        for (int i = 2; i < argc; i++)
        {
            if (access(argv[i], F_OK) == 0)
            {
                pthread_create(&tid[count], NULL, categorize, (void *)argv[i]); // categorize file

                printf("File %d : Berhasil Dikategorikan\n", i - 1);

                count++;
            }
            else
                printf("File %d : Sad, gagal :(\n", i - 1);
        }

        // as wait, join thread to wait for terminated
        for (int i = 0; i < count; i++)
            pthread_join(tid[i], NULL); // join thread

        return 0;
    }

    // soal3b
    else if (argc == 3 && strcmp(argv[1], "-d") == 0)
    {
        DIR *directoryFile = opendir(argv[2]);

        if (directoryFile)
        {
            struct dirent *dirPath;

            int threadSize = 0;
            while ((dirPath = readdir(directoryFile)) != NULL)
                if (dirPath->d_type == DT_REG) // check if it's regular file
                    threadSize++;              // adjust thread size

            categorizeFolder(argv[2], threadSize); // recursive

            closedir(directoryFile);
            printf("Direktori sukses disimpan!\n");
        }
        else if (ENOENT == errno)
            printf("Yah, gagal disimpan :(\n"); // directory does not exist
    }

    // soal3c
    else if (argc == 2 && strcmp(argv[1], "*") == 0)
    {
        char *current = getenv("PWD"); // get the path where the program runs
        DIR *dir = opendir(current);

        struct dirent *dirPath;
        int threadSize = 0;

        while ((dirPath = readdir(dir)) != NULL)
            if (dirPath->d_type == DT_REG) // check if it's regular file
                threadSize++;              // adjust thread size

        categorizeFolder(current, threadSize); // call the categorizeFolder function
        closedir(dir);
    }

    else
    {
        printf("Format input salah\n");

        return 0;
    }
}
