
struct node_t {
    int key;
    struct node_t *next, *prev;
};

struct hashtable_t {
    int size, used;
    struct node_t **table;
};

struct hashtable_t *create_ht();
void delete_ht(struct hashtable_t *ht);
void ht_insert(struct hashtable_t *ht, int key);
struct node_t *ht_search(struct hashtable_t *ht, int key);
