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

void perror_exit(char *message);

int main(int argc, char* argv[]){

   int port, sock, i;
   struct sockaddr_in server;
   struct sockaddr *serverptr = (struct sockaddr*)&server;
   struct hostent *rem;

    if(argc != 7){
        perror("Wrong arguments");
        exit(1);
    }

    if(strcmp(argv[1], "-i") != 0){
       perror("Wrong arguments"); 
    }
    char* server_ip = argv[2];

    if(strcmp(argv[3], "-p") != 0){
       perror("Wrong arguments"); 
    }
    port = atoi(argv[4]);

    if(strcmp(argv[5], "-d") != 0){
       perror("Wrong arguments"); 
    }
    char* directory = argv[6];
    // strcpy(directory, argv[6]);

    printf("Client's parameters are:\n");
    printf("serverIP: %s\n", server_ip);
    printf("port: %d\n", port);
    printf("directory: %s\n", directory);
    printf("Connected to %s on port %d\n", server_ip, port);

   /* Create socket */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      perror_exit("socket");
   
   /* Find server address */
   if ((rem = gethostbyname(server_ip)) == NULL) {
      herror("gethostbyname"); exit(1);
   }

   server.sin_family = AF_INET; /* Internet domain */
   memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
   
   server.sin_port = htons(port); /* Server port */

   /* Initiate connection */
   if (connect(sock, serverptr, sizeof(server)) < 0)
      perror_exit("connect");
   
   if (write(sock, directory, strlen(directory) + 1) < 0)
         perror_exit("write");

   int nread;

   char buf[10000];
   char size[1000];
   char text[10000];
   char temp_buf[10000];
 
   do {
      memset(buf,0,strlen(buf));
      memset(temp_buf,0,strlen(temp_buf));
      memset(size, 0, strlen(size));
      memset(text, 0, strlen(text));

      while(strlen(buf) == 0){
         if ((nread = read(sock, buf, 10000)) < 0)
            perror_exit("read");
      }
      
      // if(strlen(buf) == 0)
      //    continue;

      if(strcmp(buf, "END") == 0)
         break;

      strcpy(temp_buf, buf);

      printf("Received: %s/%s\n", directory, buf);

      char target[10000] = {0};

      strcat(target, "../output/");
      strcat(target, buf);

      if (write(sock, "OK NAME ", 10) < 0)
            perror_exit("write");

      // while(strlen(size) == 0){
      //    if(strcmp(buf, "END") == 0)
      //       break;
      //    if ((nread = read(sock, &size, 10000)) < 0)
      //      perror_exit("read");
      // }

      // printf("SIZE %s\n", size);

      // if (write(sock, "OK SIZE", 10) < 0)
      //    perror_exit("write");

      if(access(target, F_OK) == 0){
         remove(target);
      }

      FILE *trg;

      trg = fopen(target, "w");

      while(1){
            while(strlen(text) == 0){
               if((nread = read(sock, text, 1000)) < 0)
                  perror("read");
            }
   
            fprintf(trg, "%s", text);

            if(strcmp(text, "EOF") == 0){
               break;
            }

            if(strlen(text) == 0)
               break;
   
            memset(text, 0, strlen(text));

      }

      if (write(sock, "OK", 10) < 0)
         perror_exit("write");

      fclose(trg);

      if(strcmp(text, "EOF") == 0)
         continue;

   } while ((strcmp(temp_buf, "END") != 0) && (strcmp(text, "END") != 0) && (strcmp(size, "END") != 0)); /* Finish on "end" */
   
   close(sock); /* Close socket and exit */
}

void perror_exit(char *message){
   perror(message);
   exit(EXIT_FAILURE);
}