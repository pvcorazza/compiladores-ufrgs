#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"
#include "symbol_table.h"

extern FILE *yyin;
extern char *yytext;
extern int yylex();
extern int isRunning();
extern int getLineNumber();
extern int initMe();

int main(int argc, char **argv) {

    int tok;
    if (argc < 2) {
        fprintf(stderr, "Please, call: etapa1 file_name. \n");
        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0) {
        fprintf(stderr, "Cannot open file %s. \n", argv[1]);
        exit(2);
    }
    initMe();

    while (isRunning()) {
        tok = yylex();
        switch (tok) {
            case KW_CHAR:
                printf("Line %d: KW_CHAR [%s]\n", getLineNumber(), yytext);
                break;
            case KW_INT:
                printf("Line %d: KW_INT [%s]\n", getLineNumber(), yytext);
                break;
            case KW_FLOAT:
                printf("Line %d: KW_FLOAT [%s]\n", getLineNumber(), yytext);
                break;
            case KW_IF:
                printf("Line %d: KW_IF [%s]\n", getLineNumber(), yytext);
                break;
            case KW_THEN:
                printf("Line %d: KW_THEN [%s]\n", getLineNumber(), yytext);
                break;
            case KW_ELSE:
                printf("Line %d: KW_ELSE [%s]\n", getLineNumber(), yytext);
                break;
            case KW_WHILE:
                printf("Line %d: KW_WHILE [%s]\n", getLineNumber(), yytext);
                break;
            case KW_FOR:
                printf("Line %d: KW_FOR [%s]\n", getLineNumber(), yytext);
                break;
            case KW_READ:
                printf("Line %d: KW_READ [%s]\n", getLineNumber(), yytext);
                break;
            case KW_RETURN:
                printf("Line %d: KW_RETURN [%s]\n", getLineNumber(), yytext);
                break;
            case KW_PRINT:
                printf("Line %d: KW_PRINT [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_LE:
                printf("Line %d: OPERATOR_LE [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_GE:
                printf("Line %d: OPERATOR_GE [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_EQ:
                printf("Line %d: OPERATOR_EQ [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_NE:
                printf("Line %d: OPERATOR_NE [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_AND:
                printf("Line %d: OPERATOR_AND [%s]\n", getLineNumber(), yytext);
                break;
            case OPERATOR_OR:
                printf("Line %d: OPERATOR_OR [%s]\n", getLineNumber(), yytext);
                break;
            case ',':
            case ';':
            case ':':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '+':
            case '-':
            case '*':
            case '/':
            case '<':
            case '>':
            case '=':
            case '!':
            case '&':
            case '$':
            case '#':
                printf("Line %d: Caractere Especial [%s]\n", getLineNumber(), yytext);
                break;
            case TK_IDENTIFIER:
                printf("Line %d: TK_IDENTIFIER [%s]\n", getLineNumber(), yytext);
                break;
            case LIT_INTEGER:
                printf("Line %d: LIT_INTEGER [%s]\n", getLineNumber(), yytext);
                break;
            case LIT_REAL:
                printf("Line %d: LIT_REAL [%s]\n", getLineNumber(), yytext);
                break;
            case LIT_CHAR:
                printf("Line %d: LIT_CHAR [%s]\n", getLineNumber(), yytext);
                break;
            case LIT_STRING:
                printf("Line %d: LIT_STRING [%s]\n", getLineNumber(), yytext);
                break;
            case TOKEN_ERROR:
                printf("Line %d: Erro léxico [%s] \n", getLineNumber(), yytext);
                break;
            default:
                printf("(Não deveria chegar aqui) Line %d, Símbolo [%s]\n", getLineNumber(), yytext);

        }
    }
    printf("\n --- Debug symbol table: ---\n");
    hash_print();
    exit(0);

}
