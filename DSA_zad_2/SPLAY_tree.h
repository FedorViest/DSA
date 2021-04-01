//
// Created by PC on 29/03/2021.
//

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
    struct node *parent;
}node;

typedef struct splay_tree {
    struct node *root;
}splay_tree;


node* new_node(int data);
splay_tree* new_splay_tree();
void left_rotate(splay_tree *t, node *x);
void right_rotate(splay_tree *t, node *x);
void splay(splay_tree *t, node *n);
void splay_insert(splay_tree *t, node *n);
node* splay_search(splay_tree *t, node *n, int x);
void print_splay_rotations();
