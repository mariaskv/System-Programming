#include "../include/worker.h"
#include "../include/listener.h"
#include "../include/ADTQueue.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#define MSGSIZE 10000

static bool flag = false;
static bool termination = false;

void handler(int i){
    flag = true;
}
void init(int i){
    signal(SIGINT, SIG_IGN);
    exit(1);
}

int main(int argc, char* argv[]){

    int p[2];
    int rsize = 0;
    char inbuf[1000];

    if(pipe(p) == -1){
        perror("Pipe Failed");
    }

    Queue workers = queue_create();

    int fifo_name;
    int pid2;
    int pid = fork();
    if(pid == 0){
        dup2(p[1], 1);
        char* array[] = {"./listener", NULL};
        execvp(array[0], array);
    }
    else if(pid > 0){
        dup2(p[0], 0);
        while((rsize=read(p[0],inbuf,MSGSIZE)) != -1){
            
            inbuf[rsize] = '\0';
		
            if(queue_size(workers) == 0){
                pid2 = fork();
                if(pid2 == 0){ //child  	
		   worker(inbuf, getpid(), 0);   
                  break;
                }
            }
              else{
                Node node = malloc(sizeof(node));
                node = queue_get_first(workers);
                pid2 = get_node_value(node);
                kill(pid2, SIGCONT);               
             }                  
                
                signal(SIGCHLD, handler);
                if(flag == true){
                   int p;
                   int status;
                   waitpid(-1,&status,WNOHANG | WUNTRACED);
                   flag = false;
                }
                            
	   	 char* pid_name = malloc(100000*sizeof(char));
		 sprintf(pid_name, "%d", pid2);
		 sprintf(pid_name, "%s%s", pid_name, ".fifo");

		 mkfifo(pid_name, 0666);
		  
		 int wr = open(pid_name, O_WRONLY, 0666);
		 int sz = write(wr, inbuf, 100000);
		 unlink(pid_name);
             queue_insert(workers, pid2);
        }     
    }
}	


