#define SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_REAL 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 7

typedef struct table_entry {
	char *text;
	int type;
  	struct table_entry *next;
} hash_entry;

hash_entry* hash_table[SIZE];
int hash_generate(void);
int hash_address(char* text);
hash_entry* hash_insert(int type, char* text);
void hash_print(void);