
//Prevziata implementacia Chaining Hash Table

//zdroj: https://github.com/kuwairo/Hashtable
//Datum: 29.3.2021

#include "CHAINING_hash.h"

#include "stdlib.h"
#include "string.h"

#define HT_INITIAL_SIZE 11


static inline int hash(int key, int size) {
    long hash = 0;
    for (int i = 0; i < 5; ++key, ++i) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ key;
        hash %= size;
    }
    return (int) hash;
}

static inline int check_prime(int n) {
    if (n <= 1)
        return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

static inline int next_prime(int n) {
    while (check_prime(n) != 1)
        n++;
    return n;
}

static struct hashtable_t *create_new_sized_ht(int size) {
    struct hashtable_t *ht = malloc(sizeof(struct hashtable_t));
    ht->table = calloc((size_t) size, sizeof(struct node_t *));
    ht->size = size;
    ht->used = 0;
    return ht;
}

struct hashtable_t *create_ht() {
    return create_new_sized_ht(HT_INITIAL_SIZE);
}

static void resize_ht(struct hashtable_t *ht, int size) {
    if (ht->used > size || size < HT_INITIAL_SIZE)
        return;

    struct hashtable_t *resized_ht = create_new_sized_ht(size);
    for (int i = 0; i < ht->size; i++) {
        struct node_t *nd = ht->table[i], *next;
        while (nd) {
            ht_insert(resized_ht, nd->key);
            nd = nd->next;
        }
    }

    int temp_size = ht->size;
    ht->size = resized_ht->size;
    resized_ht->size = temp_size;

    struct node_t **temp_table = ht->table;
    ht->table = resized_ht->table;
    resized_ht->table = temp_table;

    delete_ht(resized_ht);
}

static void resize_up_ht(struct hashtable_t *ht) {
    int size = next_prime(ht->size * 2);
    resize_ht(ht, size);
}

void delete_ht(struct hashtable_t *ht) {
    for (int i = 0; i < ht->size; i++) {
        struct node_t *head = ht->table[i], *next;
        while (head) {
            next = head->next;
            free(head);
            head = next;
        }
    }

    free(ht->table);
    free(ht);
}

void ht_insert(struct hashtable_t *ht, int key) {
    if (ht->used >= ht->size * 0.7)
        resize_up_ht(ht);

    int h = hash(key, ht->size);

    struct node_t *head = ht->table[h], *prev = NULL;
    while (head) {
        prev = head;
        head = head->next;
    }

    struct node_t *nd = malloc(sizeof(struct node_t));
    nd->key = key;
    nd->prev = prev;
    nd->next = NULL;

    if (prev)
        prev->next = nd;
    else
        ht->table[h] = nd;

    ht->used++;
}

struct node_t *ht_search(struct hashtable_t *ht, int key) {
    int h = hash(key, ht->size);

    struct node_t *head = ht->table[h];
    while (head) {
        if (head->key == key)
            return head;
        else
            head = head->next;
    }
    return NULL;
}
