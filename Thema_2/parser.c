#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCTIONS 9
#define DOT_STRING_MAXSIZE 32768
#define MEMORY_QUEUE_MAXSIZE 128
#define SYMBOL_STACK_MAXSIZE 128
char* productions[PRODUCTIONS] = {
        "(M)",  //1
        "YZ",   //2
        "a",    //3
        "b",    //4
        "G",    //5 
        "*M",   //6
        "-M",   //7
        "+M",   //8 
        ""      //9
    };

char* nonterminals = "GMYZ";   //specify the first character of the string as the starting symbol
char* terminals = "ab()*-+$";
char* input = "((a-b)*(a+b))$";

/*
    Parser can handle around 64 characters in the input string, 
    any more and there will not be enough memory
*/

//the code for GraphViz dot will be generated and stored here!
char dotString[DOT_STRING_MAXSIZE] = "strict graph ST {\n"; 

#pragma region Declarations

//Syntax Table Declarations (public members)

extern struct SyntaxTable {
    int** table;
    int rows;
    int cols;
} SyntaxTable;

extern char* mapProductionRule(struct SyntaxTable*, int);
extern struct SyntaxTable* createSyntaxTable(char**,char**);
extern int productionExists(struct SyntaxTable*,char,char);
extern void printSyntaxTable(struct SyntaxTable*);

//Stack Declarations (public members)

extern struct Stack {
    int top;
    unsigned int size;
    char* array;
} Stack;
extern struct Stack* createStack(unsigned int);
extern int isEmpty(struct Stack*);
extern void push(struct Stack*,char);
extern char pop(struct Stack*);
extern char peek(struct Stack*);
extern void printStack(struct Stack*);

//Queue Declarations (public members)

extern struct Queue {
    int front, rear, size;
    unsigned capacity;
    struct Queue** childQueues;
    char** values;
} Queue;

extern struct Queue* createQueue(unsigned capacity);
extern int queueIsEmpty(struct Queue* queue);
extern void swapFront(struct Queue* queue, int i);
extern void enqueue(struct Queue* queue, char* value);
extern char* dequeue(struct Queue* queue);
extern char* front(struct Queue* queue);
extern char* rear(struct Queue* queue);
extern void printQueue(struct Queue* queue);

#pragma endregion

void fillSyntaxTable(struct SyntaxTable* stable) {
    stable->table[1][0] = 2;
    stable->table[2][0] = 3;

    stable->table[1][1] = 2;
    stable->table[2][1] = 4;

    stable->table[0][2] = 1;
    stable->table[1][2] = 2;
    stable->table[2][2] = 5;

    stable->table[3][3] = 9;

    stable->table[3][4] = 6;

    stable->table[3][5] = 7;

    stable->table[3][6] = 8;
}

int isTerminal(char symbol) {
    for (int i = 0; terminals[i]; i++) {
        if (symbol == terminals[i]) return 1;
        if (nonterminals[i]) {
            if (symbol == nonterminals[i]) return 0;
        } 
    }
    return -1;
}

void assignVarName(char* str, int isTerm) {
    static int vnp = 0;
    char letter;
    int num = vnp/26; 
    int alpha = vnp % 26; 
    if (isTerm == 0) {              //Assign uppercase letter
        letter = alpha + 65;
    }
    else if (isTerm == 1) {         //Assign lowercase letter
        letter = alpha + 97;
    }
    vnp += 1;
    str[0] = letter;
    str[1] = num + 48;
    str[2] = '\0';
}

int main(int argc, char*argv[]) {

    if (argc == 2) {
        input = argv[1];
        if (input[strlen(input) - 1] != '$') {
            input = strcat(input,"$");
        }
    } 
    else if (argc > 2) {
        printf("No more than 1 argument please!\n");
        return 1;
    }

    /* Pre-Processing */
    char* symbolMap[2];
    symbolMap[0] = nonterminals;
    symbolMap[1] = terminals;
    struct Stack* symStack = createStack(SYMBOL_STACK_MAXSIZE);
    struct Stack* inputStack = createStack(strlen(input));
    struct SyntaxTable* stable = createSyntaxTable(symbolMap,productions);
    fillSyntaxTable(stable);
    printSyntaxTable(stable);

    /*
        The queue takes no part in the parsing or any syntax analysis related work.
        It acts as memory so that the variables declared in the .dot code are properly re-used and 
        result in the correct formation of the syntax tree 
    */
    struct Queue* queue = createQueue(MEMORY_QUEUE_MAXSIZE);

/* 
    Begin Parsing 
*/

    printf("\nBeginning parsing of string: \"%s\"\n",input);

    // 1 - Initialize symbol stack
    push(symStack,'$');
    push(symStack,nonterminals[0]);
    
    // 2 - Fill input stack with the characters of the input string (backwards)
    for (int i = strlen(input) - 1; i >= 0; i--) {
        push(inputStack,input[i]);
    }

    // 3 - Start Parsing

    char* curprod;
    while (1) {
        char cursym = peek(symStack);       //Current symbol at the top of the symbol stack
        char curinsym = peek(inputStack);   //Current symbol at the top of the input stack

        printf("--------------------------------\n");
        printf("Current SYMBOL stack: ");
        printStack(symStack);
        printf("\nCurrent INPUT stack: ");
        printStack(inputStack);
        printf("\n--------------------------------\n");

/*
    I - Terminal symbol is matched from the symbol stack
        Pop the top element from both the input and symbol stack
*/
        if (isTerminal(cursym) == 1 && cursym == curinsym && curinsym != '$') {
            printf("\tMatched terminal: %c\n",cursym);

            pop(inputStack);
            pop(symStack);
        }
/*
    II - The terminal symbol "$" is matched from the symbol stack
        Break from the loop and terminate program
        
        Syntax Tree Generation:
            Write the finished dotString to a .dot file and execute the dot command
            to generate the .png image
*/
        else if (isTerminal(cursym) == 1 && cursym == curinsym && cursym == '$') {
            printf("\tEnd of string hit! Exiting\n");

            strcat(dotString,"}");

            FILE *fp = fopen("syntaxTree.dot","w");
            if (fputs(dotString,fp) <= 0) {
                printf("There was an error with writing .dot file!");
            }
            fclose(fp);

            /*
                Windows: https://graphviz.org/download/
                Linux: apt install graphviz
            */
            system("dot -Tpng -o syntaxtree.png syntaxTree.dot");
            break;
        }
/*
    III - Non-terminal symbol is matched and the current production being analyzed exists.
        Pop the top element from the symbol stack and for each right-hand side symbol,
        push it to the symbol stack.
*/
        else if (isTerminal(cursym) == 0 && (curprod = mapProductionRule(stable,productionExists(stable,cursym,curinsym))) != 0) {
            printf("\tMatched non-terminal: %c\n",cursym);

            char prodRule[16];
            sprintf(prodRule,"%c -> %s\n",cursym,curprod);

            if (curprod == "") {
                printf("\tCorresponding production rule: %c -> ε\n",cursym);
                sprintf(prodRule,"%c -> ε\n",cursym);
            }
            else {
                printf("\tCorresponding production rule: %s",prodRule);
            }
            
            pop(symStack);

            //Add a comment before the variable and label assignments for readability
            strcat(dotString,"\n");
            strcat(dotString,"// ");
            strcat(dotString,prodRule);

            /* Left-hand side of the .dot variable assignment (e.g: A0 -- b0, A0 is lhs) */
            char* lhs = malloc(3);
            strcpy(lhs,"XX");

            char *f = malloc(4);
            strncpy(f,front(queue),3);

            /*  
                If the front of the queue with size > 2 is not empty, but the current symbol being analyzed 
                DOES NOT match the symbol at the front of the memory queue, swap the front with the memory 
                in (front + offset) UNTIL the current symbol finally matches the current front symbol
            */
            if(strcmp(f,"XXX") != 0 && f[0] != cursym && queue->size >= 2) {
                int offset = 1;
                while(f[0] != cursym && offset <= queue->size) {
                    swapFront(queue,offset-1);
                    swapFront(queue,offset);
                    strncpy(f,front(queue),3);
                    offset++;
                }
            }
            /*
                If the front of the queue is not empty and furthermore the current symbol being analyzed 
                DOES match the symbol at the front of the memory queue, perform a check to see whether the 
                front and rear contain the SAME symbol, if yes swap those two elements.
                Without this check, variable names would be assigned to the wrong symbols and would end up
                producing a false syntax tree
            */
            if(strcmp(f,"XXX") && queue->size >= 2 && f[0] == cursym) {
                if(f[0] == rear(queue)[0]) {
                    swapFront(queue,queue->rear-queue->front);
                    strncpy(f,front(queue),3);
                }
            }

            /*
                After any possible swapping was performed by the above if statements, check to see
                if the front of the queue is not empty and the current symbol being analyzed matches 
                the front symbol.
                If yes, assign the current front value of the queue to LHS and dequeue.
            */
            if(strcmp(f,"XXX") != 0 && f[0] == cursym) {
                //read from memory queue
                char *frontValue = dequeue(queue);
                lhs = frontValue + 1; //shift str by 1 byte 
            }
            //Assign new variable name if memory is empty
            else if (queueIsEmpty(queue)){ 
                assignVarName(lhs,0);
            }
            /*
                LOOP through every element in the right-hand side of the current 
                production rule being analyzed
                - Push the symbol at index i to the symbol stack 
                - Generate the code for the .dot file using a queue
            */
            for(int i = strlen(curprod) - 1; i >= 0; i--) {
                push(symStack,curprod[i]);
                char varAssignment[16];
                char labelAssignment[40];

                //The current production symbol in left to right order
                char curprodsym = curprod[strlen(curprod)-i-1]; 

                char* rhs = malloc(3);
                assignVarName(rhs,isTerminal(curprodsym));

                /* .dot Variable declaration */

                sprintf(varAssignment,"\t%s -- %s;\n",lhs,rhs);
                strcat(dotString,varAssignment);

                /* .dot Labelling variables */ 

                sprintf(labelAssignment,"\t%s [label=\"%c\"];\n",rhs,curprodsym);
                if (isTerminal(curprodsym) == 1) {
                    sprintf(labelAssignment,"\t%s [label=\"%c\"];\n\t%s [color=red];\n",rhs,curprodsym,rhs);
                }
                strcat(dotString,labelAssignment);

                //In the last iteration, label the left-hand side symbol
                if (i == 0) {
                    sprintf(labelAssignment, "\t%s [label=\"%c\"];\n",lhs,cursym);
                    strcat(dotString,labelAssignment);
                }   

                /*
                    Store the current NONTERMINAL symbol and its .dot variable name in memory.
                    This is to properly link the syntax tree nodes.
                */
               if(isTerminal(curprodsym) == 0)
               {
                    //write to memory queue
                    char* str = malloc(4);
                    str[0] = curprodsym;
                    str[1] = rhs[0];
                    str[2] = rhs[1];
                    str[3] = '\0';
                    enqueue(queue,str);
               }
            }

        }

/*
     IV - Non-terminal symbol is matched and the current production does not exist.
*/ 
        else if ((curprod = mapProductionRule(stable,productionExists(stable,cursym,curinsym))) == 0) {
            printf("The input string is not matched by this grammar!\n");
            break;
        }
/*
    V - Undefined behavior
*/
        else {
            printf("Strange!\n");
            break;
        }
    }
    return 0;
}