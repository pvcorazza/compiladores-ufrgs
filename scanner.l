/*
  Anderson Lentz da Silva
  Paulo Victor Corazza
*/

%{

#include "tokens.h"
#include "symbol_table.h"

int running = 1;
int lineNumber = 1;
void initMe(void);
int insert_token(int token);

//tabela de simbolos
symbol_table_t* tabela;


%}

%x INSIDE

%%

 /* Palavras Reservadas */

"char"      return KW_CHAR;
"int"       return KW_INT;
"float"     return KW_FLOAT;
"if"        return KW_IF;
"then"      return KW_THEN;
"else"      return KW_ELSE;
"while"     return KW_WHILE;
"for"       return KW_FOR;
"read"      return KW_READ;
"print"     return KW_PRINT;
"return"    return KW_RETURN;


 /*Caracteres Especiais*/

"," return yytext[0];
";" return yytext[0];
":" return yytext[0];
"(" return yytext[0];
")" return yytext[0];
"[" return yytext[0];
"]" return yytext[0];
"{" return yytext[0];
"}" return yytext[0];
"+" return yytext[0];
"-" return yytext[0];
"*" return yytext[0];
"/" return yytext[0];
"<" return yytext[0];
">" return yytext[0];
"=" return yytext[0];
"!" return yytext[0];
"&" return yytext[0];
"$" return yytext[0];
"#" return yytext[0];


 /*Operadores Compostos*/

"<=" return OPERATOR_LE;
">=" return OPERATOR_GE;
"==" return OPERATOR_EQ;
"!=" return OPERATOR_NE;
"&&" return OPERATOR_AND;
"||" return OPERATOR_OR;

 /*Identificadores*/

([A-Za-z]|_)+([0-9]|([A-Za-z]|_))* return insert_token(TK_IDENTIFIER);

 /*Literais*/

[0-9]+ 			        return insert_token(LIT_INTEGER);
[0-9]+\.[0-9]+ 	        return insert_token(LIT_REAL);
'.' 					return insert_token(LIT_CHAR);
\"[^\"]*\" 				return insert_token(LIT_STRING);

 /*Comentários, espaços, quebras de linha...*/

<INITIAL>{
"/*"        BEGIN(INSIDE);
}
<INSIDE>{
"*/"        BEGIN(INITIAL);

[^*\n]+     {}
"*"         {}
\n          lineNumber++;
}

"//".*      {}
\t|\r|" "   {}
\n          lineNumber++;
.           return TOKEN_ERROR;

%%

/*
	Essa função será chamada pela main testadora substituída pelo professor.
	Responsavel pelo código de inicialização necessário (por exemplo,
	inicialização da tabela hash)

	Você precisa implementar essa função mesmo que encontre outra forma de
	inicialização que não dependa dela, pois ela será chamada pelos testes automatizados
*/
void initMe(void){

    printf("inicializando\n");
    tabela = symbol_table_new();

}


int yywrap() {
    running = 0;
    return 1;
}

int insert_token(int token) {

	table_entry_t * item = NULL;
    char *lexema = (char*) malloc(sizeof(char) * (strlen (yytext)) + 12);

    if(token == LIT_STRING || token == LIT_CHAR)
    {
        strcpy(lexema,&yytext[1]);
        lexema[strlen(yytext)-2] = '\0';
    }
    else
    {
        strcpy(lexema,yytext);
    }

	item = add_symbol_table_entry(tabela,lexema,lineNumber);	//insere o lexema na tabela hash

	return token;
}

int getLineNumber(void) {
    return lineNumber;
}

int isRunning(void) {
    return running;
}