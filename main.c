#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"
#include "symbol_table.h"


extern int running;
extern FILE * yyin;
extern char * yytext;
extern int yylex();
extern symbol_table_t *tabela;



int main(int argc, char ** argv) {

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

  while (running) {
    tok = yylex();
    switch (tok) {
      case KW_CHAR: printf("Line %d: Achei um char\n", getLineNumber());
            break;
	  case KW_INT: printf("Line %d: Achei um int \n", getLineNumber());
        break;
      case KW_FLOAT: printf("Line %d: Achei um float \n", getLineNumber());
            break;
      case KW_IF: printf("Line %d: Achei um if \n", getLineNumber());
            break;
      case KW_THEN: printf("Line %d: Achei um then \n", getLineNumber());
            break;
      case KW_ELSE: printf("Line %d: Achei um else \n", getLineNumber());
            break;
      case KW_WHILE: printf("Line %d: Achei um while \n", getLineNumber());
            break;
      case KW_FOR: printf("Line %d: Achei um for \n", getLineNumber());
            break;
      case KW_READ: printf("Line %d: Achei um read \n", getLineNumber());
            break;
      case KW_PRINT: printf("Line %d: Achei um print \n", getLineNumber());
            break;
      case KW_RETURN: printf("Line %d: Achei um return \n", getLineNumber());
            break;
      case TK_IDENTIFIER: printf("Line %d: Achei um identificador \n", getLineNumber());
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
      case '$':printf("Line %d: Simbolo: %c\n", getLineNumber(),yytext[0]);
            break;
      case TOKEN_ERROR: printf("Line %d,Erro de sintaxe\n", getLineNumber());
            break;
      default: printf("Line %d, symbol %s\n", getLineNumber(), yytext);

    }
}
  printf("\n --- Debug symbol table: ---\n");
  debug_print_symbol_table(tabela);
  exit(0);

}
