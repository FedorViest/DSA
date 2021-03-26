#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "AVL_tree.h"
#include "LP_hash.h"



void test_all(int max_size, int min_value, int max_value);
void test_AVL(int max_size, int *test);
void test_LP_hash(int max_size, int *test);

int generate_random(int min, int max);
double calculate_time(clock_t start, clock_t end);

int main() {
    srand(time(0));
    int iterations = 2;
    for (int i = 0; i < iterations; i++) {
        test_all(1000000, 1, 1000000);
    }
    return 0;
}

void test_all(int max_size, int min_value, int max_value){
    int *test = (int *) calloc(max_size, sizeof(int ));     //Alokacia pola
    int random_value;
    for (int i = 0; i < max_size; i++){     //Inicializacia nahodnych hodnot v testovom poli
        random_value = generate_random(min_value, max_value);
        test[i] = random_value;
    }

    /*
     * Volanie funkcii na testovanie
     */
    test_AVL(max_size, test);
    test_LP_hash(max_size, test);

    free(test); //Uvolnenie pamate
}

void test_AVL(int max_size, int *test){
    printf("*****TESTING WITH AVL BINARY SEARCH TREE INITIATED*****\n\n");
    TREE *root = NULL;
    clock_t start, end;     //Premenne na zaciatok a koniec casu
    double time_insert, time_search;
    start = clock();
    for (int i = 0; i < max_size; i++){
        root = AVL_insert(root, test[i]);       //Volanie funkcie na vkladanie do AVL stromu
    }
    end = clock();
    time_insert = calculate_time(start, end);      //Volanie funkcie na vypocet casu, kolko zabralo vkladanie prvkov do AVL stromu

    start = clock();

    for (int i = 0; i < max_size; i++){
        if (AVL_search(root, test[i]) == NULL)      //Volanie funkcie na hladanie v AVL strome
            printf("Error finding element: %d\n", test[i]);
    }

    end = clock();

    time_search = calculate_time(start, end);   //Volanie funkcie na vypocet casu, kolko zabralo hladanie prvkov v AVL strome
    printf("\tTime passed by insert = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n\n", time_search);
    printf("*****TESTING WITH AVL BINARY SEARCH TREE FINISHED*****\n\n\n");
}

void test_LP_hash(int max_size, int *test){
    printf("*****TESTING WITH LINEAR PROBING HASH TABLE INITIATED*****\n\n");
    UNIT *hash_table = NULL;
    clock_t start, end;     //Premenne na zaciatok a koniec casu
    double time_insert, time_search;
    start = clock();
    hash_table = init_LP_hash(hash_table);
    for (int i = 0; i < max_size; i++){
        hash_table = LP_hash_insert(hash_table, test[i]);       //Volanie funkcie na vkladanie do hash tabulky
    }
    end = clock();
    time_insert = calculate_time(start, end);       //Volanie funkcie na vypocet casu, kolko zabralo vkladanie prvkov do hash tabulky


    start = clock();

    for (int i = 0; i < max_size; i++){
        if (LP_hash_search(hash_table, test[i]) == NULL)    //Volanie funkcie na hladanie v hash tabulke
            printf("Error finding element: %d\n", test[i]);
    }

    end = clock();

    time_search = calculate_time(start, end);       //Volanie funkcie na vypocet casu, kolko zabralo hladanie prvkov v hash tabulke
    printf("\tTime passed by insert = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n\n", time_search);
    printf("*****TESTING WITH LINEAR PROBING HASH TABLE FINISHED*****\n\n\n");
}

/*
 * Generovanie nahodneho cisla zo zadaneho intervalu
 */
int generate_random(int min, int max){
    return (rand() % (max - min)) + min;
}


/*
 * Vypocet casu v sekundach
 */
double calculate_time(clock_t start, clock_t end){
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}
