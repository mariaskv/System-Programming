#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
typedef struct node* Node;
typedef struct queue* Queue; 

Queue queue_create();
int queue_size(Queue queue);
void queue_insert(Queue queue, char* name);
Node queue_get_first(Queue queue);
void queue_display(Queue queue);
char* get_node_value(Node node);
