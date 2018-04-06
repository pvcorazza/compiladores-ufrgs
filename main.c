#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"

extern FILE *yyin;
extern char *yytext;
extern int yylex();
extern int initMe();
extern int yyparse();

int main(int argc, char **argv) {

    int tok;
    if (argc < 2) {
        fprintf(stderr, "Please, call: etapa2 file_name. \n");
        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0) {
        fprintf(stderr, "Cannot open file %s. \n", argv[1]);
        exit(2);
    }
    initMe();
    yyparse();
    fprintf(stderr, "Compilation Successful! \n");


    printf("\n --- Debug symbol table: ---\n");
    hash_print();
    exit(0);

}
