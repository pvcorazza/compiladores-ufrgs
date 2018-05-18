#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE *yyin;
extern char *yytext;
extern int yylex();
extern int initMe();
extern int yyparse();

int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Please, call: etapa4 nome_arquivo\n");
        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0) {
        fprintf(stderr, "Cannot open file %s. \n", argv[1]);
        exit(2);
    }

    output_file = fopen(argv[2], "w");

    initMe();

    yyparse();
    fprintf(stderr, "\nCompilation Successful! \n");

    printf("\n --- Debug symbol table: ---\n");
    hash_print();
    fclose(output_file);
    exit(0);

}
