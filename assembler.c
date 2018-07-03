/**
 *
 * -------------- COMO USAR O GERADOR DE CÓDIGO ASSEMBLER ---------------------
 *
 *  Comando usado para gerar o assembler:
 *      ./etapa6 saida.txt      (saida.txt é o programa teste do professor)
 *      -> isto gera um arquivo chamado "codigo_assembler.s"
 *
 *  Para gerar ".o" a partir do assembler:
 *      gcc -c codigo_assembler.s -o nome_do_arq_objeto.o
 *
 *  Link e gera o executavel:
 *      gcc nome_do_arq_objeto.o -o nome_do_executavel
 */

#include "assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int setup_main = 1;

void assembler_generate(TAC *tac){

    TAC *tac_original = tac;

    FILE* file;

    if(!(file = fopen("codigo_assembler.s","w"))){
        exit(5);
    }


    /*coloca o "cabeçalho inicial do arquivo assembler
     * Somente executar uma vez, no inicio (funcao main)*/
    if(setup_main){

        /* Aqui dentro está gerando o asm para um arquivo somente com a main vazia.
            int main ()
            {

            }
         */

        fprintf(file,"\t.text\n");
        fprintf(file,"\t.globl\tmain\n");
        fprintf(file,"\t.type\tmain, @function\n");
        fprintf(file,"main:\n");
        fprintf(file, ".LFB0:\n");
        fprintf(file, "\t.cfi_startproc\n");
        fprintf(file, "\tpushq\t%%rbp\n");
        fprintf(file, "\t.cfi_def_cfa_offset 16\n");
        fprintf(file, "\t.cfi_offset 6, -16\n");
        fprintf(file, "\tmovq\t%%rsp, %%rbp\n");
        fprintf(file, "\t.cfi_def_cfa_register 6\n");
        fprintf(file, "\tmovl\t$0, %%eax\n");
        fprintf(file, "\tpopq\t%%rbp\n");
        fprintf(file, "\t.cfi_def_cfa 7, 8\n");
        fprintf(file, "\tret\n");
        fprintf(file, "\t.cfi_endproc\n");
        fprintf(file, ".LFE0:\n");
        fprintf(file, "\t.size\tmain, .-main\n");
        fprintf(file, "\t.ident\t\"GCC: (Ubuntu 7.3.0-16ubuntu3) 7.3.0\"\n");
        fprintf(file, "\t.section\t.note.GNU-stack,\"\",@progbits\n");

        setup_main = 0;

    }

    for( ; tac; tac = tac->next)
    {
        switch(tac->type)
        {
            case TAC_BEGINFUN:
            /*    fprintf(file, "\n\t.globl\t_%s\n", tac->res->text);
                fprintf(file, "\t.align\t4, 0x90\n");
                fprintf(file, "_%s:\n", tac->res->text);
                fprintf(file, "\t.cfi_startproc\n");
                fprintf(file, "## BB#%d:\n", bbc);
                fprintf(file, "\tpushq\t%%rbp\n");
                fprintf(file, "Ltmp%d:\n", ltmpc);
                fprintf(file, "\t.cfi_def_cfa_offset 16\n");
                fprintf(file, "Ltmp%d:\n", ltmpc+1);
                fprintf(file, "\t.cfi_offset %%rbp, -16\n");
                fprintf(file, "\tmovq\t%%rsp, %%rbp\n");
                fprintf(file, "Ltmp%d:\n", ltmpc+2);
                fprintf(file, "\t.cfi_def_cfa_register %%rbp\n");
                fprintf(file, "\txorl\t%%eax, %%eax\n");*/
                break;
            case TAC_ADD:

                break;

        }
    }

    fclose(file);

}
