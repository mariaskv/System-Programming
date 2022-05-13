#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>

// void listener(void){
int main(void){
    char* array[] = {"/bin/inotifywait", "-m", "--format", "%w%f", "-e", "create", "-e", "moved_to", "./Inputs", NULL}; //fix the path
    execvp(array[0], array);
}
