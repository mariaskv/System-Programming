#include "../include/ADTQueue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct node{
    pid_t pid;
    Node next;
    int status;
};

struct queue{
   Node head;
   Node tail;
   int size; 
};

Queue queue_create(){
   Queue queue = malloc(sizeof(*queue)); 
   queue->head = NULL;
   queue->tail = NULL;
   queue->size = 0;
}

int queue_size(Queue queue){
    return queue->size;
}

void queue_insert(Queue queue, pid_t id){
    Node temp = malloc(sizeof(temp));
    temp->pid = id;
    temp->next = NULL;
    temp->status = 0;
    if(queue->head == NULL){
        queue->head = temp;
        queue->tail = temp;
    }
    else{
        queue->tail->next = temp;
        queue->tail = queue->tail->next;
    }
    queue->size++;
}

Node queue_get_first(Queue queue){
    Node temp = malloc(sizeof(temp));
    temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    return temp;
}

void queue_display(Queue queue){
    printf("DISPLAY\n");
    while(queue->head != NULL){
        pid_t p = queue->head->pid;
        printf("%d\n", p);
        queue->head = queue->head->next;
    }
}

pid_t get_node_value(Node node){
    return node->pid;
}
