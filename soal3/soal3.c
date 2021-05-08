#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

void make_directory(const char *name)
{
#ifdef __linux__
    mkdir(name, 777);
#else
    _mkdir(name);
#endif
}

int main(int argc, char const *argv[])
{
    make_directory("mp3");
    make_directory("wav");
    make_directory("deb");
    make_directory("rar");
    make_directory("tar.gz");
    make_directory("zip");
    make_directory("bin");
    make_directory("iso");
    make_directory("log");
    make_directory("sql");
    make_directory("tar");
    make_directory("xml");
    make_directory("apk");
    make_directory("bat");
    make_directory("exe");
    make_directory("jar");
    make_directory("py");
    make_directory("jpg");
    make_directory("png");
    make_directory("svg");
    make_directory("html");
    make_directory("php");
    make_directory("ppt");
    make_directory("pptx");
    make_directory("pps");
    make_directory("c");
    make_directory("cpp");
    make_directory("sh");
    make_directory("xlsx");
    make_directory("ods");
    make_directory("mp4");
    make_directory("avi");
    make_directory("doc");
    make_directory("docx");
    make_directory("pdf");
    make_directory("odt");
    make_directory("txt");
    
    return 0;
}
