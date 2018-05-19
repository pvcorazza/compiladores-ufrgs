//
// Created by pvcorazza on 4/21/18.
//

#ifndef AST_HEADER
#define AST_HEADER

#include "symbol_table.h"

#define MAX_SONS 4

#define AST_PROGRAMA 200
#define AST_CORPO 201
#define AST_DECL_GLOBAL 202
#define AST_VETOR_INIC 203
#define AST_VETOR_VAZIO 204
#define AST_DECL_PONTEIRO 205
#define AST_INT 206
#define AST_FLOAT 207
#define AST_CHAR 208
#define AST_DEF_FUNCAO 209
#define AST_PARAM_LIST 210
#define AST_BLOCO_COMANDOS 211
#define AST_COMAND_LIST 212
#define AST_ATRIBUICAO 213
#define AST_ATRIBUICAO_VETOR 214
#define AST_LISTA_PRINT 215
#define AST_VET 216
#define AST_EXP_PARENTESES 217
#define AST_SOMA 218
#define AST_SUB 219
#define AST_MUL 220
#define AST_DIV 221
#define AST_L 222
#define AST_G 223
#define AST_LE 224
#define AST_GE 225
#define AST_EQ 226
#define AST_NE 227
#define AST_AND 228
#define AST_OR 229
#define AST_IDENT_REFERENCIA 230
#define AST_IDENT_DERREFERENCIA 231
#define AST_ARG_FUNCAO 232
#define AST_IF 233
#define AST_IF_ELSE 234
#define AST_FOR 235
#define AST_READ 236
#define AST_RETURN 237
#define AST_WHILE 238
#define AST_PRINT 239
#define AST_SYMBOL 240
#define AST_NOT 241
#define AST_PARAM 242
#define AST_LISTA_INIC 243
#define AST_CHAMADA_FUNCAO 244

FILE *output_file;

typedef struct ast_node {
    int type;
    int line_number;
    hash_entry *symbol;
	hash_entry *point_to_symbol;	//caso a AST for ponteiro, aqui guarda a hash pra onde o ponteiro aponta
    struct ast_node* son[MAX_SONS];
} AST;

AST* astCreate(int type, hash_entry* symbol, AST* son0, AST* son1, AST* son2, AST* son3);

void astPrint(AST *node, int level);

void uncompile(AST *ast);

#endif
