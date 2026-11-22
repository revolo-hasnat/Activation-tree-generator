%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

%%

"void"                  { return VOID; }
"int"                   { return INT; }
"return"                { return RETURN; }

[a-zA-Z_][a-zA-Z0-9_]*  {
                            yylval.name = strdup(yytext);
                            return IDENTIFIER;
                         }

[0-9]+ {
    yylval.number = atoi(yytext);
    return NUMBER;
}

"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
";"                     { return ';'; }
","                     { return ','; }

[ \t\r\n]+              { /* Ignore whitespace */ }

.                       { return INVALID; }

%%