#include "assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void assembler_generate(TAC *tac){

    TAC *tac_original = tac;

    FILE* file;

    if(!(file = fopen("saida_assembler.o","a"))){
        exit(5);
    }

}
