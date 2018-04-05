%{

#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
int getLineNumber();

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

valor_inicializacao: LIT_INTEGER
	| LIT_CHAR
	| LIT_REAL
	;

valor_literal:LIT_INTEGER
	| LIT_REAL
	| LIT_CHAR
	| LIT_STRING
	;

lista_valores_inicializacao: valor_inicializacao
    | valor_inicializacao lista_valores_inicializacao;

tipos_var_globais: KW_INT
	| KW_FLOAT
	| KW_CHAR;

/*Bloco de comandos.*/
bloco_comandos: '{' lista_comandos '}'
	;

lista_comandos: /*vazio*/
	| comando lista_comandos
	;

comando:
	| atribuicao ';'
	| comando_condicional';'
	| comando_for';'
	| comando_while';'
	| comando_read';'
	| comando_print ';'
	| comando_return ';'

atribuicao: TK_IDENTIFIER '=' expressao
	| TK_IDENTIFIER'['expressao']' '=' expressao
	;

comando_condicional:
	|
	;

comando_for:
	|
	;

comando_while:
	|
	;

comando_read: KW_READ TK_IDENTIFIER;

comando_print: KW_PRINT lista_elementos_print
	|
	;

comando_return: KW_RETURN expressao
	|
	;

lista_elementos_print: elementos_print
	| lista_elementos_print elementos_print
	;

elementos_print:  LIT_STRING
	| expressao
	;

/*Regras de definicao de funcoes.*/
lista: '(' /*vazio */ ')'
	| '(' lista_parametros ')';

/*Cabeçalho: tipo retorno da funcao, nome da funcao e uma lista de parametros*/
cabecalho: tipos_var_globais TK_IDENTIFIER lista
	;


parametros:  tipos_var_globais TK_IDENTIFIER
		;


conj_funcoes: cabecalho bloco_comandos
	;

lista_parametros: parametros | parametros ',' lista_parametros;


/* Expressoes aritmeticas e logicas*/

function_call: TK_IDENTIFIER '(' function_call_args ')'
	;

function_call_args: /*vazio*/
	| expressao function_call_more_args
	;

function_call_more_args: /*vazio*/
	| ',' expressao function_call_more_args
	;

lit_char_string: LIT_CHAR
	| LIT_STRING
	;

lit_numero: LIT_INTEGER
	| LIT_REAL
	;

operandos: lit_char_string
	| lit_numero
	| TK_IDENTIFIER'['expressao']'	//vetor indexado expressao inteira
	| TK_IDENTIFIER
	| function_call
	;

expressao: operandos
	| '('expressao')'
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
	;

%%


void yyerror (char const *s)
{
    fprintf(stderr,"Line %d: %s\n",getLineNumber(),s);
    exit(3);
}
