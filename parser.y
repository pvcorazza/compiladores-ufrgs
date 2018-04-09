%{
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
int getLineNumber();

%}
%union{
	hash_entry* symbol;
}

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
%token KW_TO

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token <symbol> TK_IDENTIFIER
%token <symbol> LIT_INTEGER
%token <symbol>  LIT_REAL
%token <symbol>  LIT_CHAR
%token <symbol>  LIT_STRING
%token TOKEN_ERROR

%left '<' '>'
%left '!' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE OPERATOR_AND OPERATOR_OR
%left '+' '-'
%left '*' '/'
%right KW_THEN KW_ELSE


%%

program : corpo_programa
	;

corpo_programa: decl_var_globais corpo_programa
	| conj_funcoes corpo_programa
	| /*vazio*/
	;

/* Regras de declaracoes de variaveis globais.*/

decl_var_globais: tipos_var_globais TK_IDENTIFIER '=' valor_inicializacao ';'
	| tipos_var_globais TK_IDENTIFIER'['LIT_INTEGER']' ';'
	| tipos_var_globais TK_IDENTIFIER'['LIT_INTEGER']' ':' lista_valores_inicializacao';'
	| tipos_var_globais '#'TK_IDENTIFIER '=' valor_inicializacao ';'
	;

tipos_var_globais: KW_INT
	| KW_FLOAT
	| KW_CHAR
	;

valor_inicializacao: LIT_INTEGER
	| LIT_CHAR
	| LIT_REAL
	;

lista_valores_inicializacao: valor_inicializacao
	| valor_inicializacao lista_valores_inicializacao
	;

/*Regras de definicao de funcoes.*/

conj_funcoes: tipos_var_globais TK_IDENTIFIER '(' lista_parametros ')' bloco_comandos
	;

/* será que função pode ser do tipo string? */

lista_parametros:
	| parametros
	| parametros ',' lista_parametros;

parametros:  tipos_var_globais TK_IDENTIFIER
;

/*Bloco de comandos.*/

bloco_comandos: '{' lista_comandos '}'
;

lista_comandos: comando
	| comando ';' lista_comandos
	;

comando:
	| bloco_comandos
	| atribuicao
	| control
	| comando_read
	| comando_print
	| comando_return

atribuicao: TK_IDENTIFIER '=' expressao
	| TK_IDENTIFIER'['expressao']' '=' expressao
	;

comando_read: KW_READ TK_IDENTIFIER;

comando_print: KW_PRINT lista_elementos_print
	;

lista_elementos_print: LIT_STRING
 	| expressao
	| lista_elementos_print expressao
	| lista_elementos_print LIT_STRING
	;

comando_return: KW_RETURN expressao	;

/* Expressoes aritméticas e logicas */

expressao: TK_IDENTIFIER
	| TK_IDENTIFIER '(' function_call_args ')'
	| TK_IDENTIFIER '[' expressao ']'
	| literal
	| '(' expressao ')'
	| expressao '+' expressao
	| expressao '-' expressao
	| expressao '*' expressao
	| expressao '/' expressao
	| expressao OPERATOR_LE expressao
	| expressao OPERATOR_GE expressao
	| expressao OPERATOR_EQ expressao
	| expressao OPERATOR_NE expressao
	| expressao OPERATOR_AND expressao
	| expressao OPERATOR_OR expressao
	| expressao '>' expressao
	| expressao '<' expressao
	| '!' expressao
	| '&' TK_IDENTIFIER
	| '#' TK_IDENTIFIER
	;

literal : LIT_CHAR
	| LIT_INTEGER
	| LIT_REAL
	;

function_call_args: /*vazio*/
	| expressao function_call_more_args
	;

function_call_more_args: /*vazio*/
	| ',' expressao function_call_more_args
	;

/* Comandos de controle de fluxo */

control: KW_IF '(' expressao ')' KW_THEN comando
	| KW_IF '(' expressao ')' KW_THEN comando KW_ELSE comando
	| KW_WHILE '(' expressao ')' comando
	| KW_FOR '(' TK_IDENTIFIER '=' expressao KW_TO expressao ')' comando
	;

%%

void yyerror (char const *s)
{
    fprintf(stderr,"Line %d: %s\n",getLineNumber(),s);
    exit(3);
}
