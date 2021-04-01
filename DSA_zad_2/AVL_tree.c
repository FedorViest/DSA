
//Moja implementacia vyvazeneho binarneho vyhladavacieho stromu AVL algoritmom

#include "AVL_tree.h"

#include <stdio.h>
#include <stdlib.h>

int rotations = 0;  //Globalna pomocna premenna na trackovanie poctu rotacii pre testovanie

/*
 * Funkcia na vytvorenie noveho listu
 */

TREE *create_node(int data){
    TREE *root = (TREE *)malloc(sizeof(TREE));      //Alokacia pamate
    root->data = data;      //Vlozenie hodnoty
    root->height = MIN_HEIGHT;  //Nastavenie vysky
    root->count = 1;    //Nastavenie poctu duplikatov
    root->left = NULL;  //Nastavenie laveho dietata
    root->right = NULL; //Nastavenie praveho dietata
    return root;
}

/*
 * Funkcia na vkladanie hodnot do BVS a nasledne vyvazenie ak je potrebne
 */

TREE *AVL_insert(TREE *root, int data){
    if (root == NULL){      //Ak nie je vytvoreny list/koren, zavola sa funkcia na jeho vytvorenie
        return create_node(data);
    }

    if (data < root->data){        //Pridavanie hodnot do laveho podstromu, ak je hodnota v strome mensia ako na vstupe, funkcia sa vnara dolava
        root->left = AVL_insert(root->left, data);
    }

    else if (data > root->data){       //Pridavanie hodnot do praveho podstromu, ak je hodnota v strome vacsia ako na vstupe, funkcia sa vnara doprava
        root->right = AVL_insert(root->right, data);
    }
    else{        //Ak sa hodnota na vstupe uz nachadza v strome, zvysi sa hodnota pre duplikaty o 1
        root->count++;
        return root;
    }

    root->height = MAX(GET_HEIGHT(root->left), GET_HEIGHT(root->right)) + 1;    //Vyratanie novej vysky pre dany koren

    return balance_tree(root, data);    //Zavolanie funkcie na vybalancovanie BVS
}

/*
 * Pomocna funkcia na volanie jednotlivych rotacii stromu a balansovanie stromu
 */

TREE *balance_tree(TREE *root, int data){
    if (HEIGHT_DIFFERENCE(root) > MIN_DIFFERENCE && data < root->left->data){       //Ak je rozdiel vysok vacsia ako 1 a zaroven hodnota na vstupe je mensia ako hodnota v strome, vykona sa rotacia vlavo
        return rotate_right(root);
    }
    if (HEIGHT_DIFFERENCE(root) > MIN_DIFFERENCE && data > root->left->data){  //Ak je rozdiel vysok vacsia ako 1 a zaroven hodnota na vstupe je vacsia ako hodnota v strome, vykona sa rotacia vlavo a nasledne rotacia vpravo
        return rotate_LeftRight(root);
    }
    if (HEIGHT_DIFFERENCE(root) < -MIN_DIFFERENCE && data < root->right->data){    //Ak je rozdiel vysok mensia ako -1 a zaroven hodnota na vstupe je mensia ako hodnota v strome, vykona sa rotacia vpravo
        return rotate_RightLeft(root);
    }
    if  (HEIGHT_DIFFERENCE(root) < -MIN_DIFFERENCE && data > root->right->data){   //Ak je rozdiel vysok mensia ako -1 a zaroven hodnota na vstupe je vacsia ako hodnota v strome, vykona sa rotacia vpravo a nasledne rotacia vlavo
        return rotate_left(root);
    }
    return root;
}

/*
 * Pomocna funkcia na vykonanie rotacie vlavo
 */

TREE *rotate_left(TREE *root){
    TREE *right_child = root->right;        //Nastavenie right child
    TREE *tmp = right_child->left;          //Nastavenie grandchild od right child

    right_child->left = root;       //Nastavenie left grandchild na parent
    root->right = tmp;              //Nastavenie right child na grandchild

    root->height = update_height(root);            //Vyratanie novej vysky pre parent
    right_child->height = update_height(right_child);     //Vyratanie novej vysky pre child

    rotations++;

    return right_child;
}

/*
 * Pomocna funkcia na vykonanie rotacie vpravo
 */

TREE *rotate_right(TREE *root){
    TREE *left_child = root->left;      //Nastavenie left child
    TREE *tmp = left_child->right;      //Nastavenie grandchild od left child

    left_child->right = root;       //Nastavenie right grandchild na parent
    root->left = tmp;               //Nastavenie left child na grandchild

    root->height = update_height(root);            //Vyratanie novej vysky pre parent
    left_child->height = update_height(left_child);      //Vyratanie novej vysky pre child

    rotations++;

    return left_child;
}

/*
 * Pomocna funkcia na vykonanie rotacie vlavo a nasledne vpravo
 */

TREE *rotate_LeftRight(TREE *root){
    root->left = rotate_left(root->left);       //Zavolanie rotacie vlavo pre left child
    return rotate_right(root);                  //Zavolanie rotacie vpravo pre parent
}

/*
 * Pomocna funkcia na vykonanie rotacie vpravo a nasledne vlavo
 */

TREE *rotate_RightLeft(TREE *root){
    root->right= rotate_right(root->right);     //Zavolanie rotacie vpravo pre right child
    return rotate_left(root);                   //Zavolanie rotacie vlavo pre parent
}

/*
 * Pomocna funkcia na aktualizovanie vysok v strome, vybera sa vacsia z vysok z left a right child a zvysuje sa o 1
 */

int update_height(TREE *root){
    return MAX(GET_HEIGHT(root->right), GET_HEIGHT(root->left)) + 1;
}

/*
 * Pomocna funkcia na vypis rotacii
 */

void print_rotations(){
    printf("\tAmount of rotations: %d\n\n", rotations);
    rotations = 0;
}

/*
 * Funkcia na hladanie hodnot v BVS, ako argumenty dostava pociatocny uzol a vstupnu hodnotu
 */

TREE *AVL_search(TREE *root, int data){
    if (root == NULL)       //Kontrola ci strom nie je prazdny
        return NULL;

    if (root->data == data)       //Ak sa rovna hodnota v strome so vstupnou hodnotou
        return root;
    else if (data < root->data)        //Ak je vstupna hodnota mensia ako hodnota v strome, funkcia sa vnara vlavo
        return AVL_search(root->left, data);
    else                          //Ak je vstupna hodnota vacsia ako hodnota v strome, funkcia sa vnara vpravo
        return AVL_search(root->right, data);
}
