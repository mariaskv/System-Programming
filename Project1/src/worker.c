#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void worker(char* buf, pid_t pid, int i){

	while(1){          
	    	
	    	char filename[10000];
	  
	   	char* pid_name = malloc(100000*sizeof(char));
		sprintf(pid_name, "%d", getpid());
		sprintf(pid_name, "%s%s", pid_name, ".fifo");
					
		int rd = open(pid_name, O_RDONLY, 0666);
		 	   
		int nread = read(rd, filename, 10000);
		unlink(pid_name);       
	    	
	    	filename[nread] = '\0';
	    	filename[strlen(filename) - 1] = '\0';
	    
		int open_id = open(filename, O_RDONLY, 0666);
		printf("OPEN: %d\n", open_id);
		if(open_id < 0){
		    perror("Open Failed");
		    exit(1);
		}

		char* token;
		token = strtok(filename, "./");
		token = strtok(NULL, "/");


		char* output =  malloc(10000*sizeof(char));
		char* new_name = malloc(10000*sizeof(char));
		char* new = malloc(10000*sizeof(char));
		strcpy(output, "./Output/");
		strcpy(new_name, token);
		strcpy(new, ".out");
		strcat(output, new_name);
		strcat(output, new);

		int create_id = open(output, O_CREAT | O_APPEND | O_WRONLY, 0666);

		char* str_read = malloc(sizeof(char));
		char* checker = NULL;
		
		char* input = malloc(100*sizeof(char));
		char** results = malloc(1000*sizeof(char*));
		for(int i = 0; i < 1000; i++){
		    results[i] = malloc(1000*sizeof(char));
		}

		int sequency[1000];
		for(int i = 0; i < 1000; i++){
		    sequency[i] = 0;
		}
		int count = 0;
		int count1 = 0;
		int pos = 0;

		memset(input,0,strlen(input));
		while(read(open_id, str_read, 1)){
			
		    if((strcmp(str_read, "\n") == 0) || ((strcmp(str_read, " ") == 0))){
		        char* checker = strstr(input, "http://");


		        if(checker == input){
		            strcpy(results[count1], input);
		            sequency[count1]++;
		            count1++;
		        }
		            memset(input,0,strlen(input));
		            checker = NULL;
		            count = 0;
		    }
		    else{
		        input[count] = str_read[0];
		        count++;
		    }
		}

		char** new_results = malloc(1000*sizeof(char*));

		for(int i = 0; i < count1; i++){
			new_results[i] = strtok(results[i], "//");
			new_results[i] = strtok(NULL, "/");
		}
		
		for(int i = 0; i < count1; i++){
		    char snum[100];
		    write(create_id, new_results[i],  strlen(new_results[i]));

		    sprintf(snum, "%d", sequency[i]);

		    write(create_id, " ",  1);
		    write(create_id, snum,  1);
		    write(create_id, "\n",  1);
		}
		    
		    raise(SIGSTOP);
	}

}






