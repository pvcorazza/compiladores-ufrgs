#define SIZE 997

typedef struct table_entry {
	char *text;
	int type;
  	struct table_entry *next;
} hash_entry;

hash_entry* hash_table[SIZE];
int generate_hash(void);
int hash_address(char* text);
hash_entry* hash_insert(int type, char* text);
void hashPrint(void);