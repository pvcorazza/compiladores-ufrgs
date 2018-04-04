#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "scanner.l"

int hash_generate() {
    int i;
    for (i=0; i<SIZE; i++) {
        hash_table[i]=0;
    }
}

int hash_address(char* text) {
    int address = 1;
    int i;
    for (i=0;i<strlen(text);i++) {
        address = (address*text[i])%SIZE + 1;
    }
    return address-1;
}

hash_entry* hash_insert(int type, char* text) {
    int address;
    hash_entry *node=0;
    address = hash_address(text);
    node = calloc(1,sizeof(hash_table));
    node->text = calloc(strlen(text)+1, sizeof(char));
    node->type = type;
    strcpy(node->text,text);
    node->next=hash_table[address];
    hash_table[address]=node;
    return node;

}

void hash_print(void) {
    int i;
    hash_entry *node;
    for (i=0; i<SIZE; i++) {
        for (node = hash_table[i]; node; node = node->next) {
            printf("Id: %d\t\t", i);
            switch (hash_table[i]->type) {
                case SYMBOL_IDENTIFIER: printf("TK_IDENTIFIER\t");
                    break;
                case SYMBOL_LIT_INT: printf("LIT_INTEGER\t\t");
                    break;
                case SYMBOL_LIT_REAL: printf("LIT_REAL\t\t");
                    break;
                case SYMBOL_LIT_CHAR: printf("LIT_CHAR\t\t");
                    break;
                case SYMBOL_LIT_STRING: printf("LIT_STRING\t\t");
                    break;
                default: printf("ERRO\t");
            }

            printf("[%s]\n", hash_table[i]->text);
        }
    }
}