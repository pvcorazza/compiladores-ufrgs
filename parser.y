%{

#include <stdio.h>
#include <stdlib.h>

%}

%define parse.error verbose
%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_READ
%token KW_RETURN
%token KW_PRINT

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token TK_IDENTIFIER
%token LIT_INTEGER
%token LIT_REAL
%token LIT_CHAR
%token LIT_STRING
%token TOKEN_ERROR

%%

program : decl

	;

decl: dec decl
	|
	;


dec: decl_var_globais
	|
	;

/* Regras de declaracoes de variaveis globais.*/
decl_var_globais: TK_IDENTIFIER ':' tipos_var_globais '=' valor_inicializacao ';'
	| TK_IDENTIFIER ':' tipos_var_globais'['LIT_INTEGER']' ';'
	| TK_IDENTIFIER ':' tipos_var_globais'['LIT_INTEGER']' lista_valores_literais';'
	;

valor_inicializacao: LIT_INTEGER
	| LIT_REAL
	| LIT_CHAR
	| LIT_REAL;

inicializacao_vetor:

valor_literal:LIT_INTEGER
	| LIT_REAL
	| LIT_CHAR
	| LIT_REAL
	| LIT_STRING;

lista_valores_literais: valor_literal
    | valor_literal lista_valores_literais;

tipos_var_globais: KW_INT
	| KW_FLOAT
	| KW_CHAR;

%%


void yyerror (char const *s)
{
    fprintf(stderr,"Line %d: %s\n",getLineNumber(),s);
    exit(3);
}
