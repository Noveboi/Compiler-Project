%{
#include "Tokens.h"
%}

%%
"Point"|"σημείο"            return POINT;
"Line"|"ευθεία"             return LINE;
"Triangle"|"τρίγωνο"        return TRIANGLE;
"Rectangle"|"τετράγωνο"     return RECTANGLE;
"Pentagon"|"πεντάγωνο"      return PENTAGON;
"Hexagon"|"εξάγωνο"         return HEXAGON;
"Heptagon"|"επτάγωνο"       return HEPTAGON;
"Octagon"|"οκτάγωνο"        return OCTAGON;

[A-Z]+                      return POINTFAMILY;

[ \t\n]                     ;
.                           return -1;
%%

int yywrap() { return 2; }