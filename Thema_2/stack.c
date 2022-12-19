#include <stdio.h>
#include <stdlib.h>

/*
    Stack implementation taken from:
    https://www.geeksforgeeks.org/introduction-to-stack-data-structure-and-algorithm-tutorials/
*/

struct Stack {
    int top;
    unsigned int size;
    char* array;
};

/// @brief Initialize an empty stack with given size 
struct Stack *createStack(unsigned int size) {
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->size = size;
    stack->top = -1;
    stack->array = malloc(sizeof(char) * size);
    return stack;
}

/// @return 0 if stack is not full, 1 otherwise. 
int isFull(struct Stack* stack) {
    return stack->top == stack->size - 1;
}

/// @return 0 if stack is not empty, 1 otherwise.
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

void push(struct Stack* stack, char symbol) {
    if(isFull(stack)) {
        printf("Stack is full!\n");
        return;
    }
    stack->array[++stack->top] = symbol;
}

char pop(struct Stack* stack) {
    if(isEmpty(stack)) return 27;
    return stack->array[stack->top--];
}

char peek(struct Stack* stack) {
    if(isEmpty(stack)) return 27;
    return stack->array[stack->top];
}

void printStack(struct Stack* stack) {
    for (int i = 0; i <= stack->top; i++) {
        printf("%c",stack->array[i]);
    }
}