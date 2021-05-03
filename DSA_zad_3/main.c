#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define COUNT 10

#define POW2 50
#define ULLI unsigned long long int


typedef struct vector{
    char *vector;
    int length;
}VECTOR;

typedef struct node{
    VECTOR *data;
    struct node *left;
    struct node *right;
}NODE;

typedef struct BDD{
    int node_number;
    int function_number;
    NODE *node;
}BDD;

ULLI* pow_of_two();
int is_pow(int size);

void test(int arg_num);

VECTOR *create_vector(char *str);
NODE *create_node(VECTOR *vector);
BDD *create_BDD(VECTOR *vector);
char use_BDD(BDD *diagram, char *str);
int reduce_BDD(BDD *diagram, NODE *root, NODE *zero, NODE *one);
NODE *rule1(NODE *root, NODE *zero, NODE *one);
BDD *rule2(BDD *diagram, NODE *root);

NODE *create_one(VECTOR *vector);
NODE *create_zero(VECTOR *vector);

char *generate_binary(int number, int length);
char **generate_table(int size);

void print2DUtil(NODE *root, int space);
void print2D(BDD *root);
void delete_tree(NODE *root);

double calculate_time(clock_t start, clock_t end);

int main() {
    srand(time(0));
    for (int j = 0; j < 5; j++)
        for (int i = 13; i < 21; i++) {
            printf("========================Test in set %d=============================\n", j);
            test(i);
        }
    return 0;
}

/*
 * Pomocna funkcia na vytvorenie vektora zo stringu, string zo vstupu sa skopiruje do stringu v strukture a nastavi sa dlzka vektora
 */
VECTOR *create_vector(char *str){
    VECTOR *data = (VECTOR *) malloc(sizeof(VECTOR));
    data->vector = strdup(str);
    data->length = strlen(str);

    return data;
}

/*
 * Funkcia na vytvorenie node. Ako argument funkcie je vektor
 */
NODE *create_node(VECTOR *vector){
    NODE *root = (NODE *) malloc(sizeof(NODE));
    root->data = vector;        //Data v strukture sa nastavia na vektor zo vstupu

    root->left = NULL;          //Lavy child sa nastavi na NULL
    root->right = NULL;         //Pravy child sa nastavi na NULL

    int length = root->data->length / 2;    //Pomocna premenna, ktora sa nastavi na polovicu dlzky stringu

    char *tmp_l, *tmp_r;    //Pomocne polia

    tmp_l = (char *) calloc(length + 1, sizeof(char ));
    tmp_r = (char *) calloc(length + 1, sizeof(char ));

    /*
     * Cyklus na naplnenie pomocnych stringov po znakoch, prva polka vektora sa nastavi do laveho stringu, druha polka vektora do praveho stringu
     */
    if (length >= 1){
        for (int i = 0; i < length * 2; i++){
            if (i < length){
                tmp_l[i] = root->data->vector[i];
            }
            else{
                tmp_r[i - length] = root->data->vector[i];
            }

        }

        tmp_l[length] = '\0';   //Nastavenie konca stringu
        tmp_r[length] = '\0';   //Nastavenie konca strginu

        /*
         * Vytvorenie vektorov z kazdeho z pomocnych stringov
         */
        VECTOR *left = create_vector(tmp_l);
        VECTOR *right = create_vector(tmp_r);


        /*
         * Rekurzia na naplnenie stromu najprv vpravo, potom vlavo
         */
        root->left = create_node(left);
        root->right = create_node(right);
    }

    /*
     * Uvolnenie pomocnych stringov
     */
    free(tmp_l);
    free(tmp_r);

    return root;
}

/*
 * Funkcia na vytvorenie celeho diagramu
 */
BDD *create_BDD(VECTOR *vector){
    BDD *root = (BDD *) calloc(1, sizeof(BDD));
    root->node = create_node(vector);   //Nastavenie prveho uzlu v strome, tak aby ukazovatel v diagrame nanho ukazoval
    root->node_number = 2 * vector->length - 1;     //Vypocet poctu uzlov v diagrame
    int iter = 1;
    int temp = vector->length;
    /*
     * Cyklus na vyratanie poctu premennych
     */
    while (temp != iter){
        temp /= 2;
        root->function_number++;
    }
    return root;
}

/*
 * Funkcia na hladanie v BDD, ako argumenty funkcie su diagram a string, ktory je prvy uzol diagramu
 */
char use_BDD(BDD *diagram, char *str){
    NODE *node = diagram->node;
    if (diagram == NULL)
        return -1;
    char result;
    while (strlen(str) >= 1 && node->data->length != 1) {
        if (str[0] == '0')
            node = node->left;  //Ak je prvy znak stringu '0', funkcia sa vnara hlbsie vlavo do stromu
        else
            node = node->right; //Ak je prvy znak stringu '1', funkcia sa vnara hlbsie vpravo do stromu
        str = str + 1;  //Posunutie sa po stringu dopredu po kazdej iteracii cyklu
    }
    result = node->data->vector[0];     //Vratenie prveho znaku v stringu

    return result;
}

/*
 * Funkica na redukciu BDD, tato funkcia sluzi ako handler na volanie funkcii podla jednotlivych kriterii. Funkcia vracia pocet uzlov v BDD
 */
int reduce_BDD(BDD *diagram, NODE *root, NODE *zero, NODE *one){
    diagram->node = rule1(root, zero, one);
    diagram->node_number -= (int)strlen(diagram->node->data->vector);
    diagram->node_number += 2;

    diagram = rule2(diagram, diagram->node);
    return diagram->node_number;
}

/*
 * Rekurzivna funkcia kde sa uvolnia vsetky uzly, ktore su naspodku stromu a nahradia sa iba dvoma dopredu vytvorenymi
 * Kedze v BDD ma kazdy uzol naspodu hodnotu '0' alebo '1', staci tieto uzly uvolnit a prenastavit ukazovatele na lavy/pravy podstrom
 */
NODE *rule1(NODE *root, NODE *zero, NODE *one){
    if (strlen(root->data->vector) > 1){
        rule1(root->left, zero, one);   //Rekurzivne vnorenie vlavo
        if (strlen(root->left->data->vector) == 1 || strlen(root->right->data->vector) == 1) {  //Rekurzia sa vnara dokym string v uzle nie je dlzky 1
            if (strcmp(root->left->data->vector, "0\0") == 0) {     //Ak sa string v uzle rovna '0', lavy ukazovatel na podstrom sa nastavi na uzol 'zero'
                free(root->left->data->vector);
                free(root->left);
                root->left = zero;
            }
            if (strcmp(root->left->data->vector, "1\0") == 0) {     //Ak sa string v uzle rovna '1', pravy ukazovatel na podstrom sa nastavi na uzol 'one'
                free(root->left->data->vector);
                free(root->left);
                root->left = one;
            }
            if (strcmp(root->right->data->vector, "0\0") == 0) {    //Ak sa string v uzle rovna '0', lavy ukazovatel na podstrom sa nastavi na uzol 'zero'
                free(root->right->data->vector);
                free(root->right);
                root->right = zero;
            }
            if (strcmp(root->right->data->vector, "1\0") == 0){     //Ak sa string v uzle rovna '1', pravy ukazovatel na podstrom sa nastavi na uzol 'one'
                free(root->right->data->vector);
                free(root->right);
                root->right = one;
            }
        }
        rule1(root->right, zero, one);      //Rekurzivne vnorenie vpravo
    }
    return root;
}

/*
 * Druhe kriterium, v ktorom sa uvolni jeden z uzlov, ktory nie je potrebny. Vzdy ked sa hodnoty 2 uzlov rovnaju, pravy z tychto uzlov sa uvolni a ukazovatele sa prenastavia
 * na ten, co zostal
 */
BDD *rule2(BDD *diagram, NODE *root){

    if (root == NULL)
        return NULL;
    NODE*right;
    if (root->data->length >= 4){       //Pordmienka na skoncenie rekurzie
        rule2(diagram, root->left);     //Vnorenie vlavo
        rule2(diagram, root->right);    //Vnorenie vpravo
        if (strcmp(root->left->data->vector, root->right->data->vector) == 0){      //Aks a hodnoty 2 uzlov ronaju
            if (root->left->right->data->length != 1 || root->right->left->data->length != 1) {     //Ak nie je dieta dietata string s dlzkou 1, lebo by sa mohli uvolnit tieto uzly, ktore su klucove pri pracovani s BDD
                free(root->right->left);    //Uvolnenie
                diagram->node_number -= 1;  //Znizenie poctu uzlov v diagrame o 1
            }
            right = root->right;        //Uvolnenie
            root->right = root->left;   //Nastavenie ukazovatela na uzol, ktory nebol uvolneny
            free(right);    //Uvolnenie
            diagram->node_number -= 1;  //Znizenie poctu uzlov v diagrame o 1
        }
    }

    return diagram;
}

/*
 * Pomocna funkcia na vytvorenie uzlu s hodnotou '0', tento uzol sa vyuziva pri redukcii podla prveho kriteria
 */
NODE *create_one(VECTOR *vector){
    NODE *node = create_node(vector);
    return node;
}

/*
 * Pomocna funkcia na vytvorenie uzlu s hodnotou '1', tento uzol sa vyuziva pri redukcii podla prveho kriteria
 */
NODE *create_zero(VECTOR *vector){
    NODE *node = create_node(vector);
    return node;
}

/*
 * Pomocna funkcia na vygenerovanie binarneho cisla, z cisla na vstupe s dlzkou aka je potrebna (napriklad ak je cislo 1, a dlzka 4, binarne cislo bude 0001)
 */
char *generate_binary(int number, int length){
    char *table = (char *) calloc(length + 1, sizeof(char));
    for (int i = length - 1; i >= 0; i--){
        unsigned int bin = number >> i;
        if (bin & 1)
            strcat(table, "1\0");
        else
            strcat(table, "0\0");
    }
    return table;
}

/*
 * Vygenerovanie tabulky pre lepsiu prehladnost pri vykonavani programu, tato tabulka sa pouziva na kontrolu, ci BDD USE funguje spravne
 */
char **generate_table(int size){
    ULLI *arr = pow_of_two();
    char **table = (char **) calloc(arr[size - 1], sizeof(char *));
    for (int i = 0; i < arr[size - 1]; i++) {
        *table = (char *) calloc(size + 1, sizeof(char));
    }

    for (int i = 0; i < arr[size - 1]; i++){
        table[i] = generate_binary(i, size);
    }
    return table;
}

/*
 * Ukladanie mocnin 2 do pola, do pola sa ulozi prvych 50 mocnin 2, takze cislo 2^50
 */
ULLI* pow_of_two(){
    ULLI *arr = (ULLI *) calloc(POW2, sizeof(ULLI));
    arr[0] = 2;
    for (int i = 1; i < POW2; i++){
        arr[i] = arr[i-1] * 2;
    }
    return arr;
}

/*
 * Pomocna funkcia na kontrolu, ci je cislo mocnina 2
 */
int is_pow(int size){
    ULLI *arr = pow_of_two();
    for (int i = 0; i < POW2; i++){
        if (arr[i] == size)
            return 1;
    }
    return 0;
}

/*
 * Testovac pre BDD, ako argument do funkcie pride pocet premennych
 */
void test(int arg_num){
    printf("Testing BDD with %d arguments:", arg_num);

    ULLI *pow_two = pow_of_two();
    char *str = (char *) calloc(pow_two[arg_num - 1] + 1, sizeof(char ));
    char *temp = (char *) calloc(1, sizeof(char ));

    clock_t start, end;
    double time_create, time_use, time_use_after_red, time_reduce, gen_vector;

    start = clock();
    for (int i = 0; i < pow_two[arg_num - 1]; i++){     //Vytvorenie stringu
        int number = (rand() % 2);
        sprintf(temp, "%d", number);
        strcat(str, temp);
    }
    char **table = generate_table(arg_num);     //Vytvorenie tabulky
    VECTOR *vector = create_vector(str);        //Vytvorenie vektora
    end = clock();

    gen_vector = calculate_time(start, end);    //Vyratanie casu, kolko trvalo vytvorenie vektora

    printf("\n\tGenerating vectors finished...\n");
    printf("\tTime for creation of vector: %.10lf seconds\n", gen_vector);


    start = clock();
    BDD *root = create_BDD(vector);     //Vytvorenie BDD
    end = clock();

    time_create = calculate_time(start, end);   //Vyratanie casu, kolko trvalo vytvorenie diagramu

    printf("\n\tCreating BDD finished...\n");
    printf("\tTime for creation of BDD: %.10lf seconds\n\t\twith %d nodes and %d arguments\n", time_create, root->node_number, root->function_number);

    int total_nodes = root->node_number;


    start = clock();
    for (int i = 0; i < pow_two[arg_num - 1]; i++){
        if (use_BDD(root, table[i]) != vector->vector[i])       //Volanie Use BDD
            printf("Incorrect use.\n");
    }
    end = clock();

    time_use = calculate_time(start, end);  //Vyratanie casu, kolko trvalo use diagramu

    printf("\n\tUsing BDD finished...\n");
    printf("\tTime for use of BDD: %.10lf seconds\n", time_use);

    VECTOR *v_one = create_vector("1");     //Vytvorenie vektora s hodnotou 1
    VECTOR *v_zero = create_vector("0");    //Vytvorenie vektora s hodnotou 0
    NODE *one = create_one(v_one);              //Vytvorenie uzlu s hodnotou 1
    NODE *zero = create_zero(v_zero);           //Vytvorenie uzlu s hodnotou 0

    start = clock();
    reduce_BDD(root, root->node, zero, one);    //Redukcia BDD
    end = clock();

    time_reduce = calculate_time(start, end);  //Vyratanie casu, kolko trvala redukcia diagramu

    printf("\n\tReducing BDD finished...\n");
    printf("\tTime for reducing BDD: %.10lf seconds\n", time_reduce);
    printf("\t%.5lf%% nodes reduced\n", ((float)root->node_number / (float)total_nodes) * 100);

    start = clock();
    for (int i = 0; i < pow_two[arg_num - 1]; i++){
        if (use_BDD(root, table[i]) != vector->vector[i])       //Volanie Use BDD
            printf("%d: Incorrect use.\n", i);
    }
    end = clock();

    time_use_after_red = calculate_time(start, end);    //Vyratanie casu, kolko trvalo use diagramu po redukcii

    printf("\n\tUsing BDD after reduction finished...\n");
    printf("\tTime for use BDD after reduction: %.10lf seconds\n", time_use_after_red);


    /*
     * Uvolnenie poli a vektorov
     */
    free(str);
    for (int i = 0; i < root->function_number; i++)
        free(table[i]);
    free(table);
    free(vector->vector);
    free(vector);
    free(root);

    printf("\n\tTest ended...\n\n");
}

/*
 * PREVZATA FUNKCIA z https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/ , tato funkcia nema ziaden dopad na funkcnost programu, ide cislo len o vypis stromu
 */
void print2DUtil(NODE *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s\n", root->data->vector);

    // Process left child
    print2DUtil(root->left, space);
}

/*
 * PREVZATA FUNKCIA z https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/ , tato funkcia nema ziaden dopad na funkcnost programu, ide cislo len o vypis stromu
 */
void print2D(BDD *root)
{
    // Pass initial space count as 0
    print2DUtil(root->node, 0);
}

/*
 * Pomocna funkcia na vyratanie casu v sekundach
 */
double calculate_time(clock_t start, clock_t end){
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}

/*
 * Pomocna funkcia na uvolnenie stromu
 */
void delete_tree(NODE *root){
    if (root == NULL)
        return;
    delete_tree(root->left);
    delete_tree(root->right);
    root->data = NULL;
    free(root->data);
    free(root);
}

