#include "../include/ADTQueue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct node{
    char* name;
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

void queue_insert(Queue queue, char* name){
    Node temp = malloc(sizeof(temp));
    temp->name= name;
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
        char*p = queue->head->name;
        printf("%s\n", p);
        queue->head = queue->head->next;
    }
}

char* get_node_value(Node node){
    return node->name;
}

