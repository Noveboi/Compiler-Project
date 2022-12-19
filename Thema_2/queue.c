#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//https://www.geeksforgeeks.org/introduction-and-array-implementation-of-queue/

struct Queue {
    int front, rear, size;
    unsigned capacity;
    /*
        A childQueue is linked to one of the values of the parent queue
    */
    struct Queue** childQueues;
    char** values;
};

struct Queue* createQueue (unsigned capacity) {
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    //We know that the values are all 3-long strings
    queue->values = malloc(3 * sizeof(char*) * queue->capacity);
    queue->childQueues = malloc(sizeof(struct Queue) * queue->capacity);
    return queue;
}

void swapFront(struct Queue* queue, int offset) {
    char* temp = queue->values[queue->front];
    queue->values[queue->front] = queue->values[queue->front + offset];
    queue->values[queue->front + offset] = temp;
}

int queueIsFull(struct Queue* queue) {
    return queue->size == queue->capacity;
}

int queueIsEmpty(struct Queue* queue) {
    return queue->size == 0;
}

void enqueue(struct Queue* queue, char* value) {
    if(queueIsFull(queue)) {
        printf("OVERFLOW: Queue in %p is full!\n",queue);
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->values[queue->rear] = value;
    queue->size += 1;
}

char* dequeue(struct Queue* queue) {
    if(queueIsEmpty(queue)) {
        printf("UNDERFLOW: Queue in %p is full!\n",queue);
        return "UND";
    }
    char* val = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return val;
}

//Next in line to be dequeued
char* front(struct Queue* queue) {
    if(queueIsEmpty(queue))
        return "XXX";
    return queue->values[queue->front];
}   

char* rear(struct Queue* queue) {
    if(queueIsEmpty(queue))
        return "EMPTY";
    return queue->values[queue->rear];
}

void printQueue(struct Queue* queue) {
    int i = queue->front;
    printf("FRONT");
    for(int k = 0; k < queue->rear - queue->front - 3; k++) 
        printf("-----");
    printf("----REAR\n");

    for(int k = 0; k <= queue->rear - queue->front; k++) {
        printf("%d",k);
        if (k < 10) {
            printf("    ");
            continue;
        }
        printf("   ");
    }
    printf("\n");

    while(i != queue->rear + 1) {
        if(i==queue->rear) {
            printf("%s\n",queue->values[i]);
            break;
        }
        printf("%s, ",queue->values[i]);
        i++;
    }
}

