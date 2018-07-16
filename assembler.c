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

#define COMPARACAO_EQ 0



char c;
aliasing a = { 0xffffffff };
int setup_main = 1;
int setup_global_data = 1;
int set_end_main = 1;

int qtd_int=0;//contador de declaracoes inteiras
int qtd_float=0;
int qtd_char=0;
int contador_strings = 0;
int tipo_comparacao_if =0;

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



                    setup_main = 0;

                }
                break;

            case TAC_ENDFUN:

                if(set_end_main){

                    fprintf(file, "\tpopq\t%%rbp\n");
                    fprintf(file, "\t.cfi_def_cfa 7, 8\n");
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

                //setup do inicio da area de declaracao de variaveis globais, somente no inicio
                if(setup_global_data)
                {
                    fprintf(file, "\t.data\n");
                    fprintf(file,"\t.align\t4\n");
                    setup_global_data = 0;
                }

                switch (tac->res->datatype){

                    case DATATYPE_INT:

                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t%s\n",tac->op1->text);
                        break;

                    case DATATYPE_FLOAT:

                        a.f = atof(tac->op1->text);
                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t%d\n",a.uint32);
                        break;



                    case DATATYPE_CHAR:

                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 1\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.byte\t%d\n",(int)*tac->res->text); //pega inteiro relativo ao caracter
                         break;

                    }
                break;

            case TAC_ADD:

                if(tac->op1->datatype == DATATYPE_INT){
                    if(tac->op2->datatype == DATATYPE_INT){
                        //entrou aqui temos uma soma de 2 operandos inteiros

                        //Declara temporário
                        fprintf(file, "\t.data\n");
                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t0\n");
                        fprintf(file,"\t.text\n\n");


                        //Teste para o operando 1
                        if(tac->op1->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tmovl	$%s, %%ecx\n", tac->op1->text);
                        }
                        else{
                            //verifica operando 1 é uma variavel inteira
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%ecx\n", tac->op1->text);
                            }
                        }

                        //Teste do operando 2
                        if(tac->op2->type == SYMBOL_LIT_INT){
                            fprintf(file, "\taddl	$%s,%%ecx\n",tac->op2->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\taddl	%s(%%rip), %%ecx\n", tac->op2->text);
                            }
                        }

                        //Move resultado
                        fprintf(file, "\tmovl	%%ecx, %s(%%rip)\n", tac->res->text);

                    }
                }
                break;

            case TAC_SUB:

                if(tac->op1->datatype == DATATYPE_INT){
                    if(tac->op2->datatype == DATATYPE_INT){
                        //entrou aqui temos uma subtraçao de 2 operandos inteiros

                        //Declara temporário do tipo inteiro
                        fprintf(file, "\t.data\n");
                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t0\n");
                        fprintf(file,"\t.text\n\n");

                        //Teste do operando 1
                        if(tac->op1->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tmovl	$%s, %%ecx\n", tac->op1->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%ecx\n", tac->op1->text);
                            }
                        }

                        //Teste do operando 2
                        if(tac->op2->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tsubl	$%s,%%ecx\n",tac->op2->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tsubl	%s(%%rip),%%ecx\n", tac->op2->text);
                            }
                        }

                        fprintf(file, "\tmovl	%%ecx, %s(%%rip)\n", tac->res->text);
                    }
                }

                break;

            case TAC_MUL:
                if(tac->op1->datatype == DATATYPE_INT){
                    if(tac->op2->datatype == DATATYPE_INT){
                        //entrou aqui temos uma subtraçao de 2 operandos inteiros

                        //Declara temporário do tipo inteiro
                        fprintf(file, "\t.data\n");
                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t0\n");
                        fprintf(file,"\t.text\n\n");

                        //Teste do operando 1
                        if(tac->op1->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tmovl	$%s, %%ecx\n", tac->op1->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%ecx\n", tac->op1->text);
                            }
                        }

                        //Teste do operando 2
                        if(tac->op2->type == SYMBOL_LIT_INT){
                            fprintf(file, "\timull	$%s,%%ecx\n",tac->op2->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\timull	%s(%%rip),%%ecx\n", tac->op2->text);
                            }
                        }

                        fprintf(file, "\tmovl	%%ecx, %s(%%rip)\n", tac->res->text);
                    }
                }

                break;

            case TAC_DIV:

                if(tac->op1->datatype == DATATYPE_INT){
                    if(tac->op2->datatype == DATATYPE_INT){
                        //entrou aqui temos uma soma de 2 operandos inteiros

                        //Declara temporário
                        fprintf(file, "\t.data\n");
                        fprintf(file, "\t.globl\t%s\n",
                                tac->res->text);
                        fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                        fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                        fprintf(file,"%s:\n",tac->res->text);
                        fprintf(file,"\t.long\t0\n");
                        fprintf(file,"\t.text\n\n");


                        //Teste para o operando 1
                        if(tac->op1->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tmovl	$%s, %%edx\n", tac->op1->text);
                        }
                        else{
                            //verifica operando 1 é uma variavel inteira
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%edx\n", tac->op1->text);
                            }
                        }

                        //Teste do operando 2
                        if(tac->op2->type == SYMBOL_LIT_INT){
                            fprintf(file, "\taddl	$%s,%%ecx\n",tac->op2->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\taddl	%s(%%rip), %%ecx\n", tac->op2->text);
                            }
                        }


                        //Faz alguma coisa pra acontecer a divisao
                        fprintf(file, "\tcltd\n");
                        fprintf(file, "\tidivl %%ecx\n");

                        //Move resultado
                        fprintf(file, "\tmovl	%%ecx, %s(%%rip)\n", tac->res->text);

                    }
                }
                break;

            case TAC_ASS:

                switch (tac->res->datatype){

                    case DATATYPE_INT:
                        fprintf(file, "\tmovl\t%%ecx, %s(%%rip)\n", tac->res->text);
                        break;

                    case DATATYPE_FLOAT:
                        a.f = atof(tac->op1->text);
                        fprintf(file, "\tmovl\t$%d, %s(%%rip)\n",a.uint32,tac->res->text);
                        break;

                    case DATATYPE_CHAR:

                        fprintf(file, "\tmovb\t$%d, %s(%%rip)\n",(int)*tac->op1->text,tac->res->text);

                        break;

                }

                break;

            case TAC_RET:

                //Testa o tipo de retorno da funcao
                switch (tac->op1->datatype){

                    case DATATYPE_INT:
                        if(tac->op1->type == SYMBOL_LIT_INT){
                            fprintf(file, "\tmovl	$%s, %%eax\n", tac->op1->text);
                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%eax\n", tac->op1->text);
                            }
                        }
                        break;
                    case DATATYPE_FLOAT:
                        if(tac->op1->type == SYMBOL_LIT_REAL){
                            printf("Retorna um float literal\n");
                            a.f = atof(tac->op1->text);
                            printf("tac->op1->text %s\n",tac->op1->text);
                            fprintf(file, "\tmovl	$%d, %%eax\n", a.uint32);
                            }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                printf("Retorna uma varivel do tipo float");
                            }
                        }
                        break;
                    case DATATYPE_CHAR:
                        if(tac->op1->type == SYMBOL_LIT_CHAR){
                            fprintf(file, "\tmovl	$%d, %%eax\n",(int)*tac->op1->text);

                        }
                        else{
                            if(tac->op1->type == SYMBOL_SCALAR){
                                fprintf(file, "\tmovl	%s(%%rip), %%eax\n", tac->op1->text);
                            }
                        }
                        break;
                    default:
                        fprintf(file, "\tmovl\t%%ecx, %%eax\n");
                        break;

                }
                break;
            case TAC_PRINT:

                if(tac->res->type == SYMBOL_LIT_STRING){

                    fprintf(file,"\t.section\t.rodata\n");
                    fprintf(file,".LC%d:\n",contador_strings);
                    fprintf(file,"\t.string\t%s\n",tac->res->text);
                    fprintf(file,"\t.text\n");

                    fprintf(file,"\tleaq\t.LC%d(%%rip), %%rdi\n",contador_strings);
                    fprintf(file,"\tmovl\t$0, %%eax\n");
                    fprintf(file,"\tcall\tprintf@PLT\n");

                    contador_strings++;
                }



                if(tac->res->type == SYMBOL_LIT_INT){

                    fprintf(file,"\t.section\t.rodata\n");
                    fprintf(file,".LC0:\n");
                    fprintf(file, "\t.string \"%%d\"\n");
                    fprintf(file,"\t.text\n");

                    fprintf(file,"\tleaq\t.LC0(%%rip), %%rdi\n");
                    fprintf(file,"\tmovl\t$0, %%eax\n");
                    fprintf(file,"\tmovl\t$%s, %%esi \n",tac->res->text);
                    fprintf(file,"\tcall\tprintf@PLT\n");



                }

                /*

                if(tac->res->type == SYMBOL_SCALAR){
                    printf("%s \n",tac->res->text);
                }


                //Chamada print sistema
                fprintf(file,"\tmovl\t$0, %%eax\n");
                fprintf(file,"\tcall\tprintf@PLT\n");*/


                break;
            case TAC_EQ:

                tipo_comparacao_if = 0; //COLOCA O TIPO DA COMPARACAO PARA O TESTE NO IF

                //Declara temporário
                fprintf(file, "\t.data\n");
                fprintf(file, "\t.globl\t%s\n",
                        tac->res->text);
                fprintf(file,"\t.type\t%s, @object\n",tac->res->text);
                fprintf(file,"\t.size\t%s, 4\n",tac->res->text);
                fprintf(file,"%s:\n",tac->res->text);
                fprintf(file,"\t.long\t0\n");
                fprintf(file,"\t.text\n\n");


                if(tac->op1->datatype == DATATYPE_INT){
                    if(tac->op2->datatype == DATATYPE_INT){



                        if(tac->op1->type == SYMBOL_SCALAR){

                            fprintf(file, "\tmovl\t%s(%%rip), %%ecx\n", tac->op1->text);

                        }
                        else{
                            fprintf(file, "\tmovl\t$%s, %%ecx\n", tac->op1->text);

                        }

                        if(tac->op2->type == SYMBOL_SCALAR){

                            fprintf(file, "\tcmpl\t%s(%%rip), %%ecx\n", tac->op2->text);
                        }
                        else{
                            fprintf(file, "\tcmpl\t$%s, %%ecx\n", tac->op2->text);

                        }

                    }
                }


                //Move para o temporario
                fprintf(file, "\tmovl	%%ecx, %s(%%rip)\n", tac->res->text);

                break;

            case TAC_IFZ:

                switch (tipo_comparacao_if){
                    case COMPARACAO_EQ:
                        fprintf(file, "\tjne\t%s\n",tac->res->text);
                        break;
                }

                break;
            case TAC_LABEL:

                fprintf(file, "%s:\n",tac->res->text);

                break;
            case TAC_JUMP:
                fprintf(file, "\tjmp\t%s\n",tac->res->text);
                break;

            case TAC_READ:
                //printf("Tipo do res: %d\n",tac->res->type);
                //printf("Datatype do res: %d\n",tac->res->datatype);
                if(tac->res->type == AST_VET){
                    printf("AST_VET");
                }

                if(tac->res->type == SYMBOL_VECTOR){
                    if(tac->res->datatype == DATATYPE_CHAR){
                        //Leitura de um array de char
                        printf("Ler uma sequencia de char - string\n");
                    }
                }
                else{
                    if(tac->res->type == SYMBOL_SCALAR){
                        switch (tac->res->datatype){
                            case DATATYPE_CHAR:
                                printf("Ler um char\n");
                                break;
                            case DATATYPE_FLOAT:
                                printf("Ler um float\n");
                                break;
                            case DATATYPE_INT:
                                printf("Ler um inteiro\n");
                                break;
                        }
                    }
                }
                break;
        }
    }





    fclose(file);

}
