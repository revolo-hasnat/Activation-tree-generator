
%{
#include <stdio.h>
#include <stdlib.h>
#include "activation.h"

int yylex(void);
void yyerror(const char *s);
%}

%union {
    char *name;
    int number;
}

%token VOID INT RETURN
%token <name> IDENTIFIER
%token <number> NUMBER
%token INVALID

%start program

%%

program:
    function_list
    {
        printf("\nParsing completed successfully!\n");
        activation_print_tree();
    }
    ;

function_list:
    function_definition
    | function_list function_definition
    ;

function_definition:
    return_type IDENTIFIER '(' ')' '{'
    {
        activation_begin_function($2);
    }
    statement_list '}'
    {
        printf("Function definition: %s\n", $2);
        activation_end_function();
        free($2);
    }
    ;

return_type:
    VOID
    | INT
    ;

statement_list:
    /* empty */
    | statement_list statement
    ;

statement:
    function_call
    | RETURN NUMBER ';'
    {
        activation_add_return($2);
    }
    | RETURN ';'
    {
        activation_add_void_return();
    }
    ;

function_call:
    IDENTIFIER '(' ')' ';'
    {
        printf("Function call: %s\n", $1);
        activation_add_call($1);
        free($1);
    }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(void)
{
    printf("Enter your C-like program:\n");

    if (yyparse() == 0)
        return 0;

    return 1;
}
