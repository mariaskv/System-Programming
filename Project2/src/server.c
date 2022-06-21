#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h> /* sockets */
#include <sys/types.h> /* sockets */
#include <sys/socket.h> /* sockets */
#include <netinet/in.h> /* internet sockets */
#include <netdb.h> /* gethostbyaddr */
#include <unistd.h> /* fork */
#include <stdlib.h> /* exit */
#include <ctype.h> /* toupper */
#include <signal.h> /* signal */
#include <dirent.h>
#include <pthread.h>
#include "../include/ADTQueue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void* child_server(void* newsock);

void perror_exit(char *message);

void sigchld_handler (int sig);

Queue tasks;

pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

int queue;

int block; 

int main(int argc, char* argv[]){

    if(argc != 9){
        perror("Wrong arguments");
        exit(1);
    }

    if(strcmp(argv[1], "-p") != 0){
       perror("Wrong arguments"); 
    }
    int port = atoi(argv[2]);

    if(strcmp(argv[3], "-s") != 0){
       perror("Wrong arguments"); 
    }
    int pool_size = atoi(argv[4]);

    if(strcmp(argv[5], "-q") != 0){
       perror("Wrong arguments"); 
    }
    queue = atoi(argv[6]);

    if(strcmp(argv[7], "-b") != 0){
       perror("Wrong arguments"); 
    }
    block = atoi(argv[8]);

    printf("Server's parameters are:\n");
    printf("port: %d\n", port);
    printf("thread_pool_size: %d\n", pool_size);
    printf("queue_size: %d\n", queue);
    printf("Block_size: %d\n", block);
    printf("Server was successfully initialized...\n");
    printf("Listening for connection to port %d\n", port);
    printf("Accepted connection from localhost\n");

   int sock, newsock;
   struct sockaddr_in server, client;
   socklen_t clientlen;
   struct sockaddr *serverptr=(struct sockaddr *)&server;
   struct sockaddr *clientptr=(struct sockaddr *)&client;
   struct hostent *rem;

   /* Reap dead children asynchronously */
   signal(SIGCHLD, sigchld_handler);

   /* Create socket */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      perror_exit("socket");
   server.sin_family = AF_INET; /* Internet domain */
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(port); /* The given port */

   /* Bind socket to address */
   if (bind(sock, serverptr, sizeof(server)) < 0)
      perror_exit("bind");

   if (listen(sock, 5) < 0) perror_exit("listen");

   tasks = queue_create();

   while (1) { 
      clientlen = sizeof(client);
      /* accept connection */
      if ((newsock = accept(sock, clientptr, &clientlen))< 0) 
         perror_exit("accept");
      /* Find client's name */
      if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,sizeof(client.sin_addr.s_addr), client.sin_family))== NULL) {
         herror("gethostbyaddr"); exit(1);
      }
         printf("Accepted connection from %s\n", rem->h_name);
         switch (fork()) { /* Create child for serving client */
         case -1: /* Error */
            perror("fork"); break;
         case 0: /* Child process */
            close(sock); 
            pthread_t t;
            int* pclient = malloc(sizeof(int));
            *pclient = newsock;
            pthread_create(&t, NULL, child_server, pclient);
            pthread_join(t, NULL);
            // child_server(&newsock);
            exit(0);
         }
      close(newsock); /* parent closes socket to client */
   }
}

void* child_server(void* sock) {
   char buf[10000];
   char temp[100000];
   int i = 0;
   int nread;

   int newsock = *(int*)sock;

   if((nread = read(newsock, buf, 1000)) < 0)
      perror("read");

   printf("[Thread %ld] : About to scan directory %s\n", pthread_self(),  buf);

   DIR *d = opendir(buf);
   struct dirent *dir;
   while((dir = readdir(d)) != NULL){
      memset(temp,0,strlen(temp));

      if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)  
         continue;

      if(queue_size(tasks) < queue){
         queue_insert(tasks, dir->d_name);
      }
      else{
         while(queue_size(tasks) >= queue);
         queue_insert(tasks, dir->d_name);
      }

      printf("[Thread %ld] : Adding file %s/%s to the queue..\n", pthread_self(), buf, dir->d_name);

      char filename[100000] = {0};

      Node node = queue_get_first(tasks);

      pthread_mutex_lock(&counter_lock);

      if (write(newsock, get_node_value(node), 100000) < 0)
         perror_exit("write");

      while(strlen(temp) == 0){
         if((nread = read(newsock, temp, 1000)) < 0)
            perror("read");
      }

      pthread_mutex_unlock(&counter_lock);

      // pthread_mutex_lock(&counter_lock);

      // int size = 1000;
      // char send[1000];
      // sprintf(send, "%d", size);
      // if (write(newsock, &send, 1000) < 0)
      //    perror_exit("write1");

      // pthread_mutex_unlock(&counter_lock);

      // memset(temp,0,strlen(temp));

      while(strlen(temp) == 0){
         if((nread = read(newsock, temp, 1000)) < 0)
            perror("read");
      }

      strcat(filename, buf);
      strcat(filename, "/");
      strcat(filename, dir->d_name);

      char* path = realpath(filename, NULL);

      FILE* src = fopen(path, "r");

      char ch[1000] = {0};
      int i = 0;
      char c;

      int count = 0;

      while((c = fgetc(src)) != EOF){
         ch[i] = c;
         i++;
         count++;
         if(count == block){
            if (write(newsock, ch, 100000) < 0)
               perror_exit("write");
            count = 0;
            ch[i] = '\0';
            i = 0;
            memset(ch, 0, strlen(ch));
         }
      }

      ch[i] = '\0';
      if (write(newsock, ch, 100000) < 0)
         perror_exit("writeCH");

      char* eof = "EOF";

      if (write(newsock, eof, 10) < 0)
         perror_exit("writeEOF");

      if((nread = read(newsock, temp, 1000)) < 0)
         perror("read");

   }

   write(newsock, "END", 100);
   closedir(d);

   printf("Closing connection.\n");
   close(newsock); /* Close socket */
   return NULL;
}

/* Wait for all dead child processes */
void sigchld_handler(int sig) {
   while (waitpid(-1, NULL, WNOHANG) > 0);
   }

void perror_exit(char *message) {
   perror(message);
   exit(EXIT_FAILURE);
}