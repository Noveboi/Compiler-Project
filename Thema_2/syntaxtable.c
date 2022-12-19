#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SyntaxTable {
    int** table;
    int rows;
    int cols;
    char* terminalMap; //Maps terminals to an index of the table
    char* nonTerminalMap; //Maps non-terminals to an index of the table
    char** productionMap;
};

/*
    Dynamic 2D array allocation taken from: 
    https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/ (Method 3)
*/

//From parser.c

/// @brief Maps a valid terminal symbol input to a table index 
/// @return The corresponding syntax table column where the terminal is located
int mapTerminal(struct SyntaxTable* sTable, char terminal) {
    int j;
    for (j = 0; sTable->terminalMap[j] != terminal; j++);
    return j;
}

/// @brief Maps a valid terminal symbol input to a table index 
/// @return The corresponding syntax table index where the terminal is located
int mapNonTerminal(struct SyntaxTable* sTable, char nonTerminal) {
    int i;
    for (i = 0; sTable->nonTerminalMap[i] != nonTerminal; i++);
    return i;
}

/// @brief Maps a valid production rule ID to the right-hand side of the production rule. 
/// @return The corresponding production rules right-hand side
char* mapProductionRule(struct SyntaxTable* sTable, int prod_id) {
    if (prod_id == 0) return 0;
    return sTable->productionMap[prod_id-1];
}

struct SyntaxTable* createSyntaxTable(char** symbolMap, char** productions) {
    struct SyntaxTable* sTable = malloc(sizeof(struct SyntaxTable));
    sTable->rows = strlen(symbolMap[0]);
    sTable->cols = strlen(symbolMap[1]);
    sTable->table = malloc(sizeof(int*) * sTable->rows);
    sTable->nonTerminalMap = symbolMap[0];
    sTable->terminalMap = symbolMap[1];
    sTable->productionMap = productions;

    for (int row = 0; row < sTable->rows; row++) {
        sTable->table[row] = malloc(sizeof(int) * sTable->cols);
    }

    for (int i = 0; i < sTable->rows; i++) {
        for (int j = 0; j < sTable->cols; j++) {
            sTable->table[i][j] = 0;
        }
    }
    return sTable;
    
}
/// @brief Find the first occurence of a production rule if specific == -1, else find the production rule that
///        corresponds to the specified column
/// @param prod_id Production Rule ID
/// @param specific Set to -1 for first occurence, or specify a column
/// @return The row and column corresponding to the given production rule, [-1,-1] if not found
int* findProduction(struct SyntaxTable* sTable, int prod_id, int specific) {
    int* location;
    location[0] = -1;
    location[1] = -1;
    if (specific != -1) {
        for (int i = 0; i < sTable->rows; i++) {
            if(sTable->table[i][specific] == prod_id) {
                location[0] = i;
                location[1] = specific;
            }
        }
    } else {
        for (int i = 0; i < sTable->rows; i++) {
            for (int j = 0; j < sTable->cols; j++) {
                if (sTable->table[i][j] == prod_id) {
                    location[0] = i;
                    location[1] = j;
                    break;
                }
            }
            if (location[i] != -1) break;
        }
    }
    return location;
}

/// @return 1 if a production rule exists, 0 otherwise
int productionExists(struct SyntaxTable* sTable, char nt, char t) {
    int prod_id;
    if ((prod_id = sTable->table[mapNonTerminal(sTable,nt)][mapTerminal(sTable,t)]) != 0) {
        return prod_id;
    }
    return 0;
}

void printSyntaxTable(struct SyntaxTable* sTable) {
    printf("  |");
    for (int t = 0; t < sTable->cols; t++) {
        printf(" %c ",sTable->terminalMap[t]);
        if (t == sTable->cols - 1) printf("\n");
    } 
    for (int l = 0; l < sTable->cols; l++) {
        printf("---");
    }
    printf("---");
    printf("\n");
    
    for (int i = 0; i < sTable->rows; i++) {
        printf("%c |",sTable->nonTerminalMap[i]);
        for (int j = 0; j < sTable->cols; j++) {
            printf(" %d ",sTable->table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}