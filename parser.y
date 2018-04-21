%{

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>


int yylex();
void yyerror(const char *s);
int getLineNumber();
AST *root;

%}
%union{
	hash_entry* symbol;
    AST* ast;
}

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


%left '!' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE OPERATOR_AND OPERATOR_OR
%left '<' '>'
%left '+' '-'
%left '*' '/'
%right KW_THEN KW_ELSE
%nonassoc TK_IDENTIFIER
%nonassoc '('

/*indica a regra de inicio*/
%start programa

%type <ast> programa
%type <ast> corpo_programa
%type <ast> decl_var_globais
%type <ast> tipos_var_globais
%type <ast> valor_inicializacao
%type <ast> lista_valores_inicializacao
%type <ast> conj_funcoes
%type <ast> lista_parametros
%type <ast> parametros
%type <ast> bloco_comandos
%type <ast> lista_comandos
%type <ast> comando
%type <ast> atribuicao
%type <ast> comando_read
%type <ast> comando_print
%type <ast> lista_elementos_print
%type <ast> comando_return
%type <ast> expressao
%type <ast> literal
%type <ast> argumentos_funcao
%type <ast> control

%%

programa : corpo_programa { root = $$; astPrint(root, 0); }
	;

corpo_programa: decl_var_globais corpo_programa { $$ = astCreate (AST_CORPO, 0, $1, $2, 0, 0); }
	| conj_funcoes corpo_programa { $$ = astCreate (AST_CORPO, 0, $1, $2, 0, 0); }
	| /*vazio*/ {$$ = 0; }
	;

/* Regras de declaracoes de variaveis globais.*/

decl_var_globais: tipos_var_globais TK_IDENTIFIER '=' valor_inicializacao ';' { $$ = astCreate(AST_DECL_GLOBAL, $2, $1, $4, 0, 0); };
	| tipos_var_globais TK_IDENTIFIER'['LIT_INTEGER']' ';' { $$ = astCreate(AST_VETOR_VAZIO, $2, $1, $4, 0, 0); }
	| tipos_var_globais TK_IDENTIFIER'['LIT_INTEGER']' ':' lista_valores_inicializacao';' { $$ = astCreate(AST_VETOR_INIC, $2, $1, $4, $7, 0); }
	| tipos_var_globais '#'TK_IDENTIFIER '=' valor_inicializacao ';' { $$ = astCreate(AST_DECL_PONTEIRO, $3, $1, $5, 0, 0); }
	;

tipos_var_globais: KW_INT { $$ = astCreate(AST_INT, 0, 0, 0, 0, 0); }
	| KW_FLOAT { $$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0); }
	| KW_CHAR { $$ = astCreate(AST_CHAR, 0, 0, 0, 0, 0); }
	;

valor_inicializacao: LIT_INTEGER { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	| LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	| LIT_REAL { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	;

lista_valores_inicializacao: valor_inicializacao
	| valor_inicializacao lista_valores_inicializacao
	;

/*Regras de definicao de funcoes.*/

conj_funcoes: tipos_var_globais TK_IDENTIFIER '(' lista_parametros ')' bloco_comandos { $$ = astCreate (AST_DEF_FUNCAO, $2, $1, $4, $6, 0); }
	;

/* será que função pode ser do tipo string? */

lista_parametros: {$$=0;}
	| parametros {$$ = $1;}
	| parametros ',' lista_parametros { $$ = astCreate(AST_PARAM_LIST, 0, $1, $3, 0, 0); };

parametros:  tipos_var_globais TK_IDENTIFIER { $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
;

/*Bloco de comandos.*/

bloco_comandos: '{' lista_comandos '}'  { $$ = astCreate(AST_BLOCO_COMANDOS, 0, $2, 0, 0, 0); };

lista_comandos: comando { $$ = $1; }
	| comando ';' lista_comandos { $$ = astCreate(AST_COMAND_LIST, 0, $1, $3, 0, 0); }
	;

comando: { $$ = 0; } //Comando Vazio
	| bloco_comandos { $$ = $1; }
	| atribuicao { $$ = $1; }
	| control { $$ = $1; }
	| comando_read { $$ = $1; }
	| comando_print { $$ = $1; }
	| comando_return { $$ = $1; }

atribuicao: TK_IDENTIFIER '=' expressao { $$ = astCreate(AST_ATRIBUICAO, $1, $3, 0, 0, 0); }
	| TK_IDENTIFIER'['expressao']' '=' expressao { $$ = astCreate(AST_ATRIBUICAO_VETOR, $1, $3, $6, 0, 0); }
	;

comando_read: KW_READ TK_IDENTIFIER { $$ = astCreate(AST_READ, $2, 0, 0, 0, 0); };

comando_print: KW_PRINT lista_elementos_print { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
	;

lista_elementos_print: LIT_STRING { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
 	| expressao { $$ = $1; }
	| lista_elementos_print expressao { $$ = astCreate(AST_LISTA_PRINT, $2, $1, 0, 0, 0); }
	| lista_elementos_print LIT_STRING { $$ = astCreate(AST_LISTA_PRINT, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0) , $1, 0, 0, 0); }
	;

comando_return: KW_RETURN expressao	{ $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
	;

/* Expressoes aritméticas e logicas */

expressao: TK_IDENTIFIER {$$ = astCreate(AST_IDENTIFICADOR,$1,0,0,0,0);}
    | TK_IDENTIFIER '(' ')' {$$ = astCreate(AST_IDENTIFICADOR,$1,0,0,0,0);}
	| TK_IDENTIFIER '(' argumentos_funcao ')' {$$ = astCreate(AST_IDENTIFICADOR,$1,$3,0,0,0);}
	| TK_IDENTIFIER '[' expressao ']'{$$ = astCreate(AST_IDENTIFICADOR,$1,$3,0,0,0);}
	| literal {$$=$1;};
	| '(' expressao ')' {$$ = astCreate(AST_EXP_PARENTESES,0,$2,0,0,0);}
	| expressao '+' expressao {$$ = astCreate(AST_SOMA,0,$1,$3,0,0);}
	| expressao '-' expressao {$$ = astCreate(AST_SUB,0,$1,$3,0,0);}
	| expressao '*' expressao {$$ = astCreate(AST_MUL,0,$1,$3,0,0);}
	| expressao '/' expressao {$$ = astCreate(AST_DIV,0,$1,$3,0,0);}
	| expressao OPERATOR_LE expressao {$$ = astCreate(AST_LE,0,$1,$3,0,0);}
	| expressao OPERATOR_GE expressao {$$ = astCreate(AST_GE,0,$1,$3,0,0);}
	| expressao OPERATOR_EQ expressao {$$ = astCreate(AST_EQ,0,$1,$3,0,0);}
	| expressao OPERATOR_NE expressao {$$ = astCreate(AST_NE,0,$1,$3,0,0);}
	| expressao OPERATOR_AND expressao {$$ = astCreate(AST_AND,0,$1,$3,0,0);}
	| expressao OPERATOR_OR expressao {$$ = astCreate(AST_OR,0,$1,$3,0,0);}
	| expressao '>' expressao {$$ = astCreate(AST_G,0,$1,$3,0,0);}
	| expressao '<' expressao {$$ = astCreate(AST_L,0,$1,$3,0,0);}
	| '!' expressao {$$ = astCreate(AST_NOT,0,$2,0,0,0);}
	| '&' TK_IDENTIFIER {$$ = astCreate(AST_IDENT_REFERENCIA,0,$2,0,0,0);}
	| '#' TK_IDENTIFIER {$$ = astCreate(AST_IDENT_DERREFERENCIA,0,$2,0,0,0);}
	;

literal : LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	| LIT_INTEGER { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	| LIT_REAL { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); };
	;

argumentos_funcao: expressao ',' argumentos_funcao { $$ = astCreate(AST_ARG_FUNCAO, 0, $1, $3, 0, 0); };
    | expressao {$$=$1;}
;

/* Comandos de controle de fluxo */

control: KW_IF '(' expressao ')' KW_THEN comando { $$ = astCreate(AST_IF, 0, $3, $6, 0, 0); };
	| KW_IF '(' expressao ')' KW_THEN comando KW_ELSE comando { $$ = astCreate(AST_IF_ELSE, 0, $3, $6, $8, 0); };
	| KW_WHILE '(' expressao ')' comando { $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0); };
	| KW_FOR '(' TK_IDENTIFIER '=' expressao KW_TO expressao ')' comando { $$ = astCreate(AST_FOR, 0, $3, $5, $7, $9); };
	;

%%

void yyerror (char const *s)
{
    fprintf(stderr,"Line %d: %s\n",getLineNumber(),s);
    exit(3);
}
