/*
  Anderson Lentz da Silva
  Paulo Victor Corazza
*/

%{
	#include "symbol_table.h"
    #include "y.tab.h"
    int running = 1;
    int lineNumber = 1;
    void initMe(void);
    int insert_token(int token);

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
"to"        return KW_TO;


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

([A-Za-z]|_)+([0-9]|([A-Za-z]|_))* { yylval.symbol = hash_insert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }

 /*Literais*/

[0-9]+ 			        { yylval.symbol = hash_insert(SYMBOL_LIT_INT, yytext); return LIT_INTEGER; }
[0-9]+\.[0-9]+ 	        { yylval.symbol = hash_insert(SYMBOL_LIT_REAL, yytext); return LIT_REAL; }
'.' 					{ yylval.symbol = hash_insert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }
\"(\\.|[^\"])*\" 		{ yylval.symbol = hash_insert(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }

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
    hash_generate();
}

int yywrap() {
    running = 0;
    return 1;
}

int getLineNumber(void) {
    return lineNumber;
}

int isRunning(void) {
    return running;
}
