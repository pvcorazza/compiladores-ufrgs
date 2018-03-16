#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"

extern int running;
extern FILE * yyin;
extern char * yytext;
extern int yylex();
extern int lineNumber;


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

  while (running) {
    tok = yylex();
    switch (tok) {
      case KW_IF: printf("Line %d: Achei um if\n", lineNumber);
            break;
      default: printf("Line %d, symbol %c\n", lineNumber, tok);

    }
}
  exit(0);

}