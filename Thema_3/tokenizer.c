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
    sToken = yylex();	//store the first token (word) recognized by yylex 
    pfToken = yylex();	//store the second token (word) recognized by yylex

    //break if there exists an unrecognized character
    if(sToken == -1 || pfToken == -1) {
        printf("Unrecognized sentence\n");
        return 1;
    }

    //break if the first token is not a shape token (point,...,octagon)
    else if(sToken < POINT || sToken > OCTAGON) {
        printf("Expected Shape, got %s\n",tokens[sToken]);
        return 2;
    }

    //break if the second token is not a point family token
    else if(pfToken != POINTFAMILY) {
        printf("Expected Point Family, got %s\n",tokens[pfToken]);
        return 3;
    }

    //break if the amount of points given does not equal the points of the given shape
    else if(yyleng != sToken) {
        printf("Mismatch, got shape: %s (%d points), but got %d points\n",tokens[sToken],sToken,yyleng);
        return 4;
    }

    //break if there is a repeated point in the point family (e.g ABA, ABCDEA, e.t.c)
    else if(repeatExists(yytext)) {
        printf("Repeated symbol in \"%s\"\n",yytext);
        return 5;
    }
    printf("Got valid shape!\n");

    return 0;
}
