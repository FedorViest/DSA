#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "AVL_tree.h"
#include "LP_hash.h"
#include "SPLAY_tree.h"
#include "CHAINING_hash.h"


void test_all(int max_size, int min_value, int max_value, int iterations);
double test_AVL(int max_size, int *test);
double test_LP_hash(int max_size, int *test);
double test_SPLAY(int max_size, int *test);
double test_CHAINING_hash(int max_size, int *test);
long long int generate_random(int min, int max);
double calculate_time(clock_t start, clock_t end);

void start_print(int max_size, int min_value, int max_value, int iterations);

int main() {
    srand(time(0));

    int iterations = 1, max_size = 1000000, min_value = 1, max_value = 10000000; //Pomocne premenne v poradi zlava doprava: pocet vykonani testu; pocet prvkov kolko chceme vlozit
                                                                                 //do struktur, najmensia hodnota, ktoru chceme vlozit; najvacsia hodnota, ktoru chceme vlozit

    start_print(max_size, min_value, max_value, iterations);


    for (int i = 0; i < iterations; i++) {
        test_all(max_size, min_value, max_value, iterations);
    }
    return 0;
}

/*
 * Hlavny testovac, ktory vola vsetky pomocne funkcie a zaroven rata priemerny cas pre kazdu y implementacii
 */

void test_all(int max_size, int min_value, int max_value, int iterations){
    int *test = (int *) calloc(max_size, sizeof(int ));     //Alokacia pola
    long long int random_value;

    for (int i = 0; i < max_size; i++){     //Inicializacia nahodnych hodnot v testovom poli
        random_value = generate_random(min_value, max_value);
        test[i] = random_value;
    }

    static double time_AVL = 0, time_hash_table = 0, time_splay = 0, time_chain = 0;    //Inicializacia premennych na vyratanie priemernych casov
    static int count;

    /*
     * Volanie funkcii na testovanie
     */
    time_AVL += test_AVL(max_size, test);
    time_splay += test_SPLAY(max_size, test);
    time_hash_table += test_LP_hash(max_size, test);
    time_chain += test_CHAINING_hash(max_size, test);

    free(test); //Uvolnenie pamate

    count++;    //Zvacsenie poctu vykonanych testov o 1

    if (count == iterations) {
        count = 0;
        printf("*****AVERAGE TIMES*****\n\n");
        printf("\tAvegrage time for insert and search in AVL Binary search tree is %.3lf\n", time_AVL / iterations);
        printf("\tAvegrage time for insert and search in Red-Black Binary search tree is %.3lf\n", time_splay / iterations);
        printf("\tAvegrage time for insert and search in Linear Probing Hash Table is %.3lf\n", time_hash_table / iterations);
        printf("\tAvegrage time for insert and search in Chaining Hash Table is %.3lf\n\n", time_chain / iterations);
        time_AVL = time_splay = time_hash_table = time_chain = 0;
    }
}

/*
 * Vsetky testovacie scenare prebiehaju rovnako, jediny rozdiel je medzi inicializaciou struktur / poli, ktore su nutne na spravny chod jednotlivych
 * programov.
 */

double test_AVL(int max_size, int *test){
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


    //Vypisy
    printf("\tTime passed by inserting = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n", time_search);
    print_rotations();
    printf("*****TESTING WITH AVL BINARY SEARCH TREE FINISHED*****\n\n\n");


    double time_total = time_insert + time_search;
    return time_total;
}

double test_SPLAY(int max_size, int *test){
    printf("*****TESTING WITH SPLAY BINARY SEARCH TREE INITIATED*****\n\n");


    struct splay_tree *root = new_splay_tree();
    struct node **new = (node **) calloc(max_size, sizeof(node*));


    clock_t start, end;
    double time_insert, time_search;


    start = clock();
    for (int i = 0; i < max_size; i++){
        new[i] = new_node(test[i]);
        splay_insert(root, new[i]);
    }
    end = clock();
    time_insert = calculate_time(start, end);


    start = clock();
    for (int i = 0; i < max_size; i++){
        if (splay_search(root, new[i], test[i]) == NULL)
            printf("Error finding element: %d\n", test[i]);
    }
    end = clock();
    time_search = calculate_time(start, end);


    printf("\tTime passed by inserting = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n", time_search);
    print_splay_rotations();
    printf("*****TESTING WITH SPLAY BINARY SEARCH TREE FINISHED*****\n\n\n");


    double time_total = time_insert + time_search;
    return time_total;
}

double test_LP_hash(int max_size, int *test){
    printf("*****TESTING WITH LINEAR PROBING HASH TABLE INITIATED*****\n\n");


    UNIT *hash_table = NULL;
    hash_table = init_LP_hash(hash_table);

    clock_t start, end;
    double time_insert, time_search;


    start = clock();
    for (int i = 0; i < max_size; i++){
        hash_table = LP_hash_insert(hash_table, test[i]);
    }
    end = clock();
    time_insert = calculate_time(start, end);


    start = clock();
    for (int i = 0; i < max_size; i++){
        if (LP_hash_search(hash_table, test[i]) == NULL)
            printf("Error finding element: %d\n", test[i]);
    }
    end = clock();
    time_search = calculate_time(start, end);


    printf("\tTime passed by inserting = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n\n", time_search);
    printf("*****TESTING WITH LINEAR PROBING HASH TABLE FINISHED*****\n\n\n");


    double time_total = time_insert + time_search;
    return time_total;
}

double test_CHAINING_hash(int max_size, int *test){
    printf("*****TESTING WITH CHAINING HASH TABLE INITIATED*****\n\n");


    struct hashtable_t *hash_table = NULL;
    hash_table = create_ht();


    clock_t start, end;
    double time_insert, time_search;


    start = clock();
    for (int i = 0; i < max_size; i++){
        ht_insert(hash_table, test[i]);
    }
    end = clock();
    time_insert = calculate_time(start, end);


    start = clock();
    for (int i = 0; i < max_size; i++){
        if (ht_search(hash_table, test[i]) == NULL)
            printf("Error finding element: %d\n", test[i]);
    }
    end = clock();
    time_search = calculate_time(start, end);


    printf("\tTime passed by inserting = %.3lf seconds\n", time_insert);
    printf("\tTime searching values = %.3lf seconds\n\n", time_search);
    printf("*****TESTING WITH CHAINING HASH TABLE FINISHED*****\n\n\n");


    double time_total = time_insert + time_search;
    return time_total;
}


/*
 * Generovanie nahodneho cisla zo zadaneho intervalu
 */
long long int generate_random(int min, int max){
    long long int result =  (rand() % (max - min)) + min;   //Najprv sa vygeneruje nahodne cislo (1-32768, lebo maximalna hodnota pre windows je 2^15)
    result = result << 15;                                  //Vygenerovane cislo pomocou bitshift posuniem
    result = result | (rand() % (max - min)) + min;         //Generovanie noveho nadodneho cisla a spojenie s predoslym (teraz ma cislo rozsah 2^30)
    result %= max;                                          //Modulovanie najvacsou hodnotou z intervalu, aby bolo vygenerovane cislo zo zadaneho intervalu
    return result;

}

/*
 * Pomocna funkcia na krajsi vypis
 */

void start_print(int max_size, int min_value, int max_value, int iterations){
    if (iterations == 1)
        printf("\n\nInserting %d elements with values from %d to %d %d time\n\n", max_size, min_value, max_value, iterations);
    else
        printf("\n\nInserting %d elements with values from %d to %d %d times\n\n", max_size, min_value, max_value, iterations);

}


/*
 * Vypocet casu v sekundach
 */
double calculate_time(clock_t start, clock_t end){
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}
