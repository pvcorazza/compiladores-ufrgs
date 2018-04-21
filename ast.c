//
// Created by pvcorazza on 4/21/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
extern FILE *output_file;

AST* astCreate(int type, hash_entry* symbol, AST* son0, AST* son1, AST* son2, AST* son3) {
    AST *node = (AST*) calloc(1, sizeof(AST));
    node->type = type;
    node->symbol = symbol;
    node->son[0] = son0;
    node->son[1] = son1;
    node->son[2] = son2;
    node->son[3] = son3;
    return node;
}

void astPrint(AST *node, int level) {
    if (!node) return;
    int i;
    for (i = 0; i < level; i++) printf("  ");
    printf("AST(");
    switch (node->type) {
        case AST_PROGRAMA:
            printf("AST_PROGRAMA");
            break;
        case AST_CORPO:
            printf("AST_CORPO");
            break;
        case AST_DECL_GLOBAL:
            printf("AST_DECL_GLOBAL");
            break;
        case AST_VETOR_INIC:
            printf("AST_VETOR_INIC");
            break;
        case AST_VETOR_VAZIO:
            printf("AST_VETOR_VAZIO");
            break;
        case AST_DECL_PONTEIRO:
            printf("AST_DECL_PONTEIRO");
            break;
        case AST_INT:
            printf("AST_INT");
            break;
        case AST_FLOAT:
            printf("AST_FLOAT");
            break;
        case AST_CHAR:
            printf("AST_CHAR");
            break;
        case AST_DEF_FUNCAO:
            printf("AST_DEF_FUNCAO");
            break;
        case AST_PARAM_LIST:
            printf("AST_PARAM_LIST");
            break;
        case AST_BLOCO_COMANDOS:
            printf("AST_BLOCO_COMANDOS");
            break;
        case AST_COMAND_LIST:
            printf("AST_COMAND_LIST");
            break;
        case AST_ATRIBUICAO:
            printf("AST_ATRIBUICAO");
            break;
        case AST_ATRIBUICAO_VETOR:
            printf("AST_ATRIBUICAO_VETOR");
            break;
        case AST_LISTA_PRINT:
            printf("AST_LISTA_PRINT");
            break;
        case AST_IDENTIFICADOR:
            printf("AST_IDENTIFICADOR");
            break;
        case AST_EXP_PARENTESES:
            printf("AST_EXP_PARENTESES");
            break;
        case AST_SOMA:
            printf("AST_SOMA");
            break;
        case AST_SUB:
            printf("AST_SUB");
            break;
        case AST_MUL:
            printf("AST_MUL");
            break;
        case AST_DIV:
            printf("AST_DIV");
            break;
        case AST_L:
            printf("AST_L");
            break;
        case AST_G:
            printf("AST_G");
            break;
        case AST_LE:
            printf("AST_LE");
            break;
        case AST_GE:
            printf("AST_GE");
            break;
        case AST_EQ:
            printf("AST_EQ");
            break;
        case AST_NE:
            printf("AST_NE");
            break;
        case AST_AND:
            printf("AST_AND");
            break;
        case AST_OR:
            printf("AST_OR");
            break;
        case AST_IDENT_REFERENCIA:
            printf("AST_IDENT_REFERENCIA");
            break;
        case AST_IDENT_DERREFERENCIA:
            printf("AST_IDENT_DERREFERENCIA");
            break;
        case AST_ARG_FUNCAO:
            printf("AST_ARG_FUNCAO");
            break;
        case AST_IF:
            printf("AST_IF");
            break;
        case AST_IF_ELSE:
            printf("AST_IF_ELSE");
            break;
        case AST_FOR:
            printf("AST_FOR");
            break;
        case AST_READ:
            printf("AST_READ");
            break;
        case AST_RETURN:
            printf("AST_RETURN");
            break;
        case AST_WHILE:
            printf("AST_WHILE");
            break;
        case AST_PRINT:
            printf("AST_PRINT");
            break;
        case AST_SYMBOL:
            printf("AST_SYMBOL");
            break;
        case AST_NOT:
            printf("AST_NOT");
            break;
        case AST_PARAM:
            printf("AST_PARAM");
            break;
        case AST_LISTA_INIC:
            printf("AST_LISTA_INIC");
            break;
        case AST_CHAMADA_FUNCAO:
            printf("AST_CHAMADA_FUNCAO");
            break;
        default:
            printf("Node Type: %d", node->type);
            break;
    }

    if (node->symbol) {
        printf(", %s)\n", node->symbol->text);
    } else {
        printf(",)\n");
    }

    for (i = 0; i < MAX_SONS; i++) {
        astPrint(node->son[i], level + 1);
    }
}

    void uncompile(AST *ast){
        if(!ast) return;
        if(ast){
            switch(ast->type){
                case AST_SYMBOL:
                    fprintf(output_file,"%s", ast->symbol->text);
                    break;
                case AST_CORPO:
                    if(ast->son[1] != NULL) {
                        uncompile(ast->son[0]);
                        fprintf(output_file," \n");
                        uncompile(ast->son[1]);
                    }
                    else
                        uncompile(ast->son[0]);
                    break;
                case AST_DECL_GLOBAL:
                    uncompile(ast->son[0]);
                    fprintf(output_file," %s = ", ast->symbol->text);
                    uncompile(ast->son[1]);
                    fprintf(output_file,";");
                    break;
                case AST_VETOR_INIC:
                    uncompile(ast->son[0]);
                    fprintf(output_file," %s [", ast->symbol->text);
                    uncompile(ast->son[1]);
                    fprintf(output_file,"] : ");
                    uncompile(ast->son[2]);
                    fprintf(output_file,";");
                    break;
                case AST_VETOR_VAZIO:
                    uncompile(ast->son[0]);
                    fprintf(output_file," %s [", ast->symbol->text);
                    uncompile(ast->son[1]);
                    fprintf(output_file,"]");
                    fprintf(output_file,";");
                    break;
                case AST_DECL_PONTEIRO:
                    uncompile(ast->son[0]);
                    fprintf(output_file," #%s = ", ast->symbol->text);
                    uncompile(ast->son[1]);
                    fprintf(output_file,";");
                    break;
                case AST_INT: 
                    fprintf(output_file,"int");
                    break;
                case AST_FLOAT:
                    fprintf(output_file,"float");
                    break;
                case AST_CHAR:
                    fprintf(output_file,"char");
                    break;
                case AST_DEF_FUNCAO:
                    uncompile(ast->son[0]);
                    fprintf(output_file," %s (", ast->symbol->text);
                    uncompile(ast->son[1]);
                    fprintf(output_file,") ");
                    uncompile(ast->son[2]); //Bloco da função
                    break;
                case AST_PARAM_LIST:
                    uncompile(ast->son[0]);
                    fprintf(output_file,",");
                    uncompile(ast->son[1]);
                    break;
                case AST_BLOCO_COMANDOS:
                    fprintf(output_file,"{ \n");
                    uncompile(ast->son[0]);
                    fprintf(output_file,"}");
                    break;
                case AST_COMAND_LIST:
                    uncompile(ast->son[0]);
                    fprintf(output_file,";\n");
                    uncompile(ast->son[1]);
                    break;
                case AST_ATRIBUICAO:
                    fprintf(output_file,"%s = ", ast->symbol->text);
                    uncompile(ast->son[0]);
                    break;
                case AST_ATRIBUICAO_VETOR:
                    fprintf(output_file,"%s[", ast->symbol->text);
                    uncompile(ast->son[0]);
                    fprintf(output_file,"] = ");
                    uncompile(ast->son[1]);
                    break;
                case AST_LISTA_PRINT:
                    uncompile(ast->son[0]);
                    fprintf(output_file," ");
                    uncompile(ast->son[1]);
                    break;
                case AST_IDENTIFICADOR:
                    fprintf(output_file,"%s", ast->symbol->text);
                    break;
                case AST_EXP_PARENTESES:
                    fprintf(output_file,"AST_EXP_PARENTESES");
                    break;
                case AST_SOMA:
                    uncompile(ast->son[0]);
                    fprintf(output_file,"+");
                    uncompile(ast->son[1]);
                    break;
                case AST_SUB:
                    uncompile(ast->son[0]); fprintf(output_file,"-"); uncompile(ast->son[1]);
                    break;
                case AST_MUL:
                    uncompile(ast->son[0]); fprintf(output_file,"*"); uncompile(ast->son[1]);
                    break;
                case AST_DIV:
                    uncompile(ast->son[0]); fprintf(output_file,"/"); uncompile(ast->son[1]);
                    break;
                case AST_L:
                    uncompile(ast->son[0]); fprintf(output_file,"<"); uncompile(ast->son[1]);
                    break;
                case AST_G:
                    uncompile(ast->son[0]); fprintf(output_file,">"); uncompile(ast->son[1]);
                    break;
                case AST_LE:
                    uncompile(ast->son[0]); fprintf(output_file,"<="); uncompile(ast->son[1]);
                    break;
                case AST_GE:
                    uncompile(ast->son[0]); fprintf(output_file,">="); uncompile(ast->son[1]);
                    break;
                case AST_EQ:
                    uncompile(ast->son[0]); fprintf(output_file,"=="); uncompile(ast->son[1]);
                    break;
                case AST_NE:
                    uncompile(ast->son[0]); fprintf(output_file,"!="); uncompile(ast->son[1]);
                    break;
                case AST_AND:
                    uncompile(ast->son[0]); fprintf(output_file,"&&"); uncompile(ast->son[1]);
                    break;
                case AST_OR:
                    uncompile(ast->son[0]); fprintf(output_file,"||"); uncompile(ast->son[1]);
                    break;
                case AST_IDENT_REFERENCIA:
                    fprintf(output_file,"&");
                    fprintf(output_file,"%s", ast->symbol->text);
                    break;
                case AST_IDENT_DERREFERENCIA:
                    fprintf(output_file,"#");
                    fprintf(output_file,"%s", ast->symbol->text);
                    break;
                case AST_ARG_FUNCAO:
                    uncompile(ast->son[0]);
                    fprintf(output_file,",");
                    uncompile(ast->son[1]);
                    break;
                case AST_IF:
                    fprintf(output_file,"if(");
                    uncompile(ast->son[0]);
                    fprintf(output_file,") then ");
                    fprintf(output_file," \n");
                    uncompile(ast->son[1]);
                    break;
                case AST_IF_ELSE:
                    fprintf(output_file,"if(");
                    uncompile(ast->son[0]);
                    fprintf(output_file,") then ");
                    uncompile(ast->son[1]);
                    fprintf(output_file,"else ");
                    uncompile(ast->son[2]);
                    break;
                case AST_FOR:
                    fprintf(output_file,"for (%s = ", ast->symbol->text);
                    uncompile(ast->son[0]);
                    fprintf(output_file," to ");
                    uncompile(ast->son[1]);
                    fprintf(output_file,") \n");
                    uncompile(ast->son[2]);
                    break;
                case AST_READ:
                    fprintf(output_file,"read ");
                    fprintf(output_file,"%s", ast->symbol->text);
                    break;
                case AST_RETURN:
                    fprintf(output_file,"return ");
                    uncompile(ast->son[0]);
                    break;
                case AST_WHILE:
                    fprintf(output_file,"while(");
                    uncompile(ast->son[0]);
                    fprintf(output_file,")");
                    uncompile(ast->son[1]);
                    break;
                case AST_PRINT:
                    fprintf(output_file,"print "); uncompile(ast->son[0]);
                    break;
                case AST_NOT:
                    fprintf(output_file,"!");uncompile(ast->son[0]);
                    break;
                case AST_PARAM:
                    uncompile(ast->son[0]);
                    fprintf(output_file," %s", ast->symbol->text);
                    break;
                case AST_LISTA_INIC:
                    uncompile(ast->son[0]);
                    fprintf(output_file," ");
                    uncompile(ast->son[1]);
                    break;
                case AST_CHAMADA_FUNCAO:
                    fprintf(output_file,"%s(", ast->symbol->text);
                    uncompile(ast->son[0]);
                    fprintf(output_file,")");
                    break;
                default:
                    fprintf(output_file,"ERRO\n");
                    break;
            }
        }

        return;

    }