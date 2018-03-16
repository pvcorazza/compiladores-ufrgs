/*
  Anderson Lentz da Silva
  Paulo Victor Corazza
*/

%{
#include "parser.h" //arquivo automaticamente gerado pelo bison
#include "cc_misc.h"
#include "cc_dict.h"

int insert_token(int token);

extern int line_count; //Variável definida em cc_misc.h para contagem de linhas
extern comp_dict_t *symbol_table; 
%}

%x INSIDE

%%

 /* Palavras Reservadas */

"int"       return TK_PR_INT;
"float"     return TK_PR_FLOAT;
"bool"      return TK_PR_BOOL;
"char"      return TK_PR_CHAR;
"string"    return TK_PR_STRING;
"if"        return TK_PR_IF;
"then"      return TK_PR_THEN;
"else"      return TK_PR_ELSE;
"while"     return TK_PR_WHILE;
"do"        return TK_PR_DO;
"input"     return TK_PR_INPUT;
"output"    return TK_PR_OUTPUT;
"return"    return TK_PR_RETURN;
"const"     return TK_PR_CONST;
"static"    return TK_PR_STATIC;
"foreach"   return TK_PR_FOREACH;
"for"       return TK_PR_FOR;
"switch"    return TK_PR_SWITCH;
"case"      return TK_PR_CASE;
"break"     return TK_PR_BREAK;
"continue"  return TK_PR_CONTINUE;
"class"     return TK_PR_CLASS;
"private"   return TK_PR_PRIVATE;
"public"    return TK_PR_PUBLIC;
"protected" return TK_PR_PROTECTED;

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
"%" return yytext[0];
"#" return yytext[0];
"^" return yytext[0];

 /*Operadores Compostos*/

"<=" return TK_OC_LE;
">=" return TK_OC_GE;
"==" return TK_OC_EQ;
"!=" return TK_OC_NE;
"&&" return TK_OC_AND;
"||" return TK_OC_OR;
"<<" return TK_OC_SL;
">>" return TK_OC_SR;

 /*Identificadores*/

([A-Za-z]|_)+([0-9]|([A-Za-z]|_))* return insert_token(TK_IDENTIFICADOR);

 /*Literais*/

[+-]?[0-9]+ 			return insert_token(TK_LIT_INT);
([+-]?[0-9]+)\.[0-9]+ 	return insert_token(TK_LIT_FLOAT);
false					return insert_token(TK_LIT_FALSE);
true					return insert_token(TK_LIT_TRUE);
'.' 					return insert_token(TK_LIT_CHAR);
\"[^\"]*\" 				return insert_token(TK_LIT_STRING);

 /*Comentários, espaços, quebras de linha...*/

<INITIAL>{
"/*"        BEGIN(INSIDE);
}
<INSIDE>{
"*/"        BEGIN(INITIAL);
[^*\n]+     {}
"*"         {}
\n          line_count++;
}

"//".*      {}
\t|\r|" "   {}
\n          line_count++;
.           return TOKEN_ERRO;

%%

int insert_token(int token){

	char *lexema = (char*) malloc(sizeof(char) * (strlen (yytext)) + 1);
	
	strcpy(lexema,yytext);

	//printf("lexema: %s, linha %d \n",lexema,line_count);

	dict_put(symbol_table,lexema,line_count);	//insere o lexema na tabela hash
	int linha = dict_get(symbol_table,lexema);
	//printf("Incluiu lexema na tabela\n");

	free(lexema);
	return token;
}