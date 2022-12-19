#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

char* str;      //The string that will be constructed 
int i;          //Points to the current index of the constructed string
long total_steps = 100;
long steps = 0;      

void cat(char);
int parse_Z();
int parse_K();
int parse_G();
int parse_M();

int main(int argc, char*argv[]) {
    //Seed the RNG every time the program is run in order to produce different result every time
    srand(time(NULL));

    //If user specifies the amount of steps in the command-line arguments, overwrite the default
    if (argc == 2) {
        total_steps = strtol(argv[1],NULL,10);
        if (total_steps > 100000) {
            printf("Please input a number of steps less than 100,000.\n");
            return 1;
        }
    }
    //Allocate up to 200KB of memory on the heap for the constructed string to use
    str = malloc(sizeof(char) * total_steps * 2);

    //Begin parsing
    if (parse_Z() == -1) {
        printf("Error with parsing Z\n");
        return 2;
    }

    printf("\nString generation completed! The output string is shown below\n");
    printf("%s\n",str);

    free(str);
    return 0;
}

void cat(char symbol) {
    str[i] = symbol;
    i++;    
}

int parse_Z() {
    if(steps >= total_steps) return 0;
    printf("Step #%li | Parsing Z\n",steps);
    
    cat('(');
    printf("\tGenerated symbol \"(\"\n");
    steps++;
    printf("Step #%li | Using production <Z> ::= (<K>)\n",steps);
    if (parse_K() == -1) {
        printf("Error with parsing K\n");
        return -1;
    }
    cat(')');
    printf("\tGenerated symbol \")\"\n");
    return 0;
}

int parse_K() {
    if(steps >= total_steps) return 0;
    printf("Step #%li | Parsing K\n", steps);
    steps++;
    printf("Step #%li | Using production <K> ::= <G><M>\n",steps);
    if(parse_G() == -1) {
        printf("Error with parsing G\n");
        return -1;
    }
    steps++;
    if(parse_M() == -1) {
        printf("Error with parsing M\n");
        return -1;
    }
    return 0;
}

int parse_G() {
    if(steps >= total_steps) return 0;
    printf("Step #%li | Parsing G\n",steps);

    int decision = rand() % 1;
    if (decision == RAND_MAX) return -1;
    if(decision == 0) {
        steps++;
        printf("Step #%li | Using production <G> ::= v\n",steps);
        cat('v');
        printf("\tGenerated symbol \"v\"\n");
    } else {
        steps++;
        printf("Step #%li | Using production <G> ::= <Z>\n",steps);
        if(parse_Z() == -1) {
            printf("Error with parsing Z\n");
            return -1;
        }
    }
    return 0;
}

int parse_M() {
    if(steps >= total_steps) return 0;
    printf("Step #%li | Parsing M\n",steps);

    int decision = rand() % 2;
    if (decision == RAND_MAX) return -1;
    steps++;

    if (decision == 0) {
        printf("Step #%li | Using production <M> ::= -<K>\n",steps);
        cat('-');
        printf("\tGenerated symbol \"-\"\n");
    } 
    else if (decision == 1) {
        printf("Step #%li | Using production <M> ::= +<K>\n",steps);
        cat('+');
        printf("\tGenerated symbol \"+\"\n");
    } 
    else {
        printf("Step #%li | Using production <M> ::= ε\n",steps);
        printf("\tGenerated symbol \"ε\"\n");
    }
    if (parse_K() == -1) {
        printf("Error with parsing K\n");
        return -1;
    }
    return 0;
    /*If decision == 3 do nothing*/
}