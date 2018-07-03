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
char c;
int setup_main = 1;
int setup_global_data = 1;
int set_end_main = 1;

void assembler_generate(TAC *tac){

    TAC *tac_original = tac;

    FILE* file;

    if(!(file = fopen("codigo_assembler.s","w"))){
        exit(5);
    }



    for( ; tac; tac = tac->next)
    {
        switch(tac->type)
        {
            case TAC_BEGINFUN:

                if(setup_main){

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

                    setup_main = 0;

                }
                break;

            case TAC_ENDFUN:

                if(set_end_main){
                    fprintf(file, "\tret\n");
                    fprintf(file, "\t.cfi_endproc\n");
                    fprintf(file, ".LFE0:\n");
                    fprintf(file, "\t.size\tmain, .-main\n");
                    fprintf(file, "\t.ident\t\"GCC: (Ubuntu 7.3.0-16ubuntu3) 7.3.0\"\n");
                    fprintf(file, "\t.section\t.note.GNU-stack,\"\",@progbits\n");

                    set_end_main = 0;
                }

                break;

            //Gera asm para declaracao de variaveis globais
            case TAC_VARDEC:

                printf("TAC_VARDEC\n");

                //setup do inicio da area de declaracao de variaveis globais, somente no inicio
                if(setup_global_data)
                {
                    fprintf(file, "\t.data\n");
                    setup_global_data = 0;
                }

                switch (tac->res->datatype){

                    case DATATYPE_INT:

                        fprintf(file, "\t.globl\t%s\n\t.align\t4\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t%s\n",tac->op1->text);
                        break;

                        /* case DATATYPE_CHAR:
                             c = *tac->res->text;
                             printf("c= %c\n",c);
                             fprintf(file, "\n\t.globl\t_%s\n\t.align\t2\n_%s:\n\t.byte\t%d \n",
                                     tac->res->text, tac->res->text,(int) c);
                             break;
                             */

                    }
                break;

            case TAC_ADD:

                break;
            case TAC_ASS:

                break;
        }
    }





    fclose(file);

}
