#include <stdio.h>
#include <string.h>
#include "Tokens.h"

extern int yylex();
extern char* yytext;
extern int yyleng;

int repeatExists(char* string) {
    for (int i = 0; string[i]; i++) {
        for (int j = 0; string[j]; j++) {
            if (i != j && string[i] == string[j]) {
                return 1;
            }
        }
    }
    return 0;
}

/*
    Built with the help of https://www.youtube.com/watch?v=54bo1qaHAfk 
    and the FLEX manual
*/

char* tokens[] = {NULL, "Point", "Line", "Triangle", "Rectangle",
                        "Pentagon", "Hexagon", "Heptagon", "Octagon","Point Family"};

int main() {
    printf("Lexing time!\n");

    int sToken, pfToken;
    sToken = yylex();
    pfToken = yylex();
    if(sToken == -1 || pfToken == -1) {
        printf("Unrecognized sentence\n");
        return 1;
    }
    else if(sToken < POINT || sToken > OCTAGON) {
        printf("Expected Shape, got %s\n",tokens[sToken]);
        return 2;
    }
    else if(pfToken != POINTFAMILY) {
        printf("Expected Point Family, got %s\n",tokens[pfToken]);
        return 3;
    }
    else if(yyleng != sToken) {
        printf("Mismatch, got shape: %s (%d points), but got %d points\n",tokens[sToken],sToken,yyleng);
        return 4;
    }
    else if(repeatExists(yytext)) {
        printf("Repeated symbol in \"%s\"\n",yytext);
        return 5;
    }
    printf("Got valid shape!\n");

    return 0;
}