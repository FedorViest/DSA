
//Moja implementacia hashovacej tabulky so zvacsovanim s algoritmom Linear Probing

#include "LP_hash.h"

#include <stdio.h>
#include <stdlib.h>

int size = MIN_SIZE;    //Globalna premenna na uchovavanie velkosti pamate

/*
 * Funkcia na inicializaciu velkosti pamate a hashovacej tabulky
 */

UNIT *init_LP_hash(UNIT *hash_table){
    if (even_size()){                                   //Ak je pociatocna velkost pamate parna, nastavi sa na neparnu
        size += 1;
    }
    hash_table = (UNIT *) calloc(size, sizeof(UNIT));   //Inicializacia hashovacej tabulky, vsade sa nastavi hodnota 0
    return hash_table;
}

/*
 * Vytvorenie hashu
 */

int LP_create_hash(int data){
    return data % size;
}

/*
 * Funkcia na pridavanie hodnot do hashovacej tabulky, vo funkcii sa taktyiez vola samostatna funkcia na zvacsenie tabulky a prehashovanie hodnot
 */

UNIT *LP_hash_insert(UNIT *hash_table, int data){
    static int occupied;                                            //staticka premenna, ktora urcuje kolko hodnot bolo zapisanych do tabulky
    int hashed_value = LP_create_hash(data);                        //Vytvorenie hash hodnoty

    while (hash_table[hashed_value].full != FREE_VALUE){            //While cyklus, ktory cykli, dokym nenajde volne miesto v tabulke
        if (hashed_value == size - 1){                              //Kontrola, ci program neprisiel na koniec pamate, ak ano, cyklus pokracuje od indexu 0, kvoli tomuto, sa vzdy najde v tabulke volne miesto
            hashed_value = 0;
        }
        if (hash_table[hashed_value].full == FREE_VALUE)            //V tomto pripade sa kontroluje, ci cyklus nasiel volne miesto v pamati, ak ano, cyklus sa ukonci a prejde sa na vkladanie hodnoty do tabulky
            break;
        if (hash_table[hashed_value].data == data){                 //Kontrola, ci uz hodnota na vstupe nie je pritomna v tabulke, ak je, funkcia skonci a caka na dalsiu hodnotu zo vstupu
            return hash_table;
        }
        hashed_value = (hashed_value + 1) % size;                   //Zvacsenie indexu
    }
    hash_table[hashed_value].data = data;                           //Priradenie hodnoty na vstupe na volnu poziciu tabulky
    hash_table[hashed_value].full = OCCUPIED_VALUE;                 //Oznacenie zaplneneho miesta v tabulke
    occupied++;                                                     //Inkrementacia o 1, kvoli pridanej hodnote do tabulky

    double percentage_occupied = (double)occupied / (double)size;   //Vyratanie percenta zaplnenosti tabulky, ak je zaplnena na viac ako 75%, zavola sa funkcia na zvacsenie tabulky
    if (percentage_occupied >= MAX_OCCUPIED){
        occupied = 0;                                               //Nastavenie premennej na 0, kedze sa vytvara nova tabulka, ktora vsetky prvky musi opat pridat do tabulky ale pod novym hashom
        hash_table = resize_table(hash_table);                      //Funkcia na zvacsenie tabulky
    }
    return hash_table;
}

/*
 * Funkcia na zvacsenie tabulky, ako argument dostava staru tabulku, vo funkcii sa vytvori nova, s vacsou velkostou a nasledne sa prekopiruju hodnoty zo starej do novej
 */

UNIT *resize_table(UNIT *hash_table){
    int old_size = size;                                        //Uchovanie starej velkosti pamate

    size = 2*old_size + 1;                                      //Zvacsenie velkosti o 2-nasobok + 1 (pseudoprvocislo)
    UNIT *new_table = (UNIT *) calloc(size, sizeof(UNIT));      //Inicializacia novej (pomocnej) tabulky

    for (int i = 0; i < old_size; i++){                         //Cyklus, ktory kopiruje hodnoty zo starej tabulky do novej
        if (hash_table[i].full == FREE_VALUE){                  //Ak je volne miesto v tabulke, cyklus ide na dasliu iteraciu
            continue;
        }
        LP_hash_insert(new_table, hash_table[i].data);          //Volanie funkcie na vkladanie do novej tabulky
    }
    free(hash_table);                                           //Uvolnenie starej tabulky

    return new_table;
}

/*
 * Funkcia na hladanie hodnoty v tabulke, ako argumenty dostava tabulku a hodnotu, ktoru treba najst
 */

UNIT *LP_hash_search(UNIT *hash_table, int data){
    int hashed_value = LP_create_hash(data);                    //Vytvorenie hashu pre hodnotu na vstupe
    int temp = hashed_value - 1;                                //Pomocna premenna na uchovanie hodnoty o 1 mensej ako je zahashovana hodnota, aby cyklus vedel, kedy presiel celu tabulku

    while (hash_table[hashed_value].full != FREE_VALUE){        //Cyklus ide pokym nenajde volne miesto v tabulke
        if (hash_table[hashed_value].data == data){             //Kontrola, ci sa nasla hodnota zo vstupu, ak ano, funkcia vrati tabulku
            return hash_table;
        }
        if (hashed_value == size - 1){                          //Osetrenie ci program nie je na konci pamate, ak je, ide od zaciatku
            hashed_value = 0;
        }
        if (temp == hashed_value)                               //Kontrola, ci som presiel celu pamat
            return NULL;
        hashed_value = (hashed_value + 1) % size;               //Zvacsenie indexu
    }
    return NULL;
}

/*
 * Pomocna funckia na zistenie, ci je cislo parne/neparne
 */
int even_size(){
    return size % 2 == 0 ? 1 : 0;
}
