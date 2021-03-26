
/*
 * Vo funkciach memory_init, memory_alloc, memory_free a coalescing su zakomentovane for-cykly, ktore sluzia na nastavovanie hodnot bajtov,
 * je to cisto pre lepsiu vizualizaciu ako sa sprava pamat
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define ALLOC_VALUE 1 //hodnota, ktorou si oznacujem zaplneny bajt
#define FREE_VALUE  -5 //hodnota, ktorou si oznacujem prazdny bajt
#define OUT_OF_MEMORY 2 //hodnota, ktorou si oznacujem koniec vyhradenej pamate
#define MINIMUM_BLOCK_SIZE 4


void *ptr_start; //Globalny ukazovatel, ktory ukazuje na zaciatok pamate

void memory_init(void *ptr, unsigned int size);
void *memory_alloc(unsigned int size);
int memory_free(void *valid_ptr);
int memory_check(void *ptr);

unsigned int size_check_init(unsigned int size);
unsigned int size_check_alloc(unsigned int size);
void *first_fit(void *ptr, unsigned int size);
void coalescing(int handler);
void memory_tester(char *region, unsigned int min_byte, unsigned int max_byte, unsigned int min_memory, unsigned int max_memory);
void print_test(char *region, int iteration); //Funkcia na vypis uspesnosti testovania
void print_percentage(int total_bytes, int allocated_bytes, int total_blocks, int allocated_blocks, int curr_memory); //Funkcia na vypocet percent v testovani

int main() {
    char region[500000];
    srand(time(0));
    print_test(region, 10);
    return 0;
}

/*
 * Vo funkcii memory_init sa nastavi pozadovana pamat. Najprv sa zaokruhli velkost na cislo delitelne 4, ak je to potrebne
 * V cykle sa postupne kazdy jeden bajt nastavi na volny
 * Po for cykle sa vytvori hlavicka na zaciatok pamate s celkovou velkostou pamate. Na 4. bajt od tejto hlavicky sa vytvori
 * dalsia, s udajom o velkosti volnej pamate. Na konci pamate sa vyhradia 4 bajty, ked sa nastavi paticka pamate.
 * Globalny ukazovatel sa nastavi na zaciatok volnej pamate
 */

void memory_init(void *ptr, unsigned int size){
    size = size_check_init(size);


    //ODSTRANIT KOMENTAR PRE NASTAVENIE BAJTOV


    /*for (int i = 0; i < size; i++){
        *((char *)(ptr + i)) = FREE_VALUE;
    }*/


    *((unsigned int *)(ptr)) = size;
    ptr_start = ptr;
    *((unsigned int *)(ptr_start + sizeof(unsigned int ))) = *((unsigned int*)(ptr_start)) - 2*sizeof(unsigned int );
    *((unsigned int *)(ptr + size - sizeof(unsigned int))) = OUT_OF_MEMORY;
    ptr_start += sizeof(unsigned int);
}

/*
 * Funkcia memory_alloc sluzi na alokaciu pamate o danej dlzke. Najprv sa pozadovana pamat zarovna nahor tak, aby jej velkost bola delitelna 4
 * Nasleduje kontrola, ci dana velkost nie je mala na to, aby sa alokovala pamat aj s hlavickou, ak ano, funkcia vracia NULL
 */

void *memory_alloc(unsigned int size){
    size = size_check_alloc(size);
    if (*(unsigned int*)ptr_start < MINIMUM_BLOCK_SIZE + sizeof(unsigned int ) || size < MINIMUM_BLOCK_SIZE){
        return NULL;
    }

    unsigned int *alloc_pointer = NULL;
    alloc_pointer = first_fit(alloc_pointer, size); //Najdenie prvej vyhovujucej pozicie v pamati, ak nenajde, funkcia vrati NULL
    if (alloc_pointer == NULL){
        return NULL;
    }
    unsigned int value = 0;
    unsigned int *new_ptr;
    value = *alloc_pointer; //Pomocna premenna na uchovanie velkost predosleho bloku, ktory sa ma alokovat

    *alloc_pointer = size + sizeof(unsigned int) + 1; //Nastavenie hodnoty ukazovatela na velkost ktoru chcem alokovat + 1, co je oznacenie zaplneneho bloku pamate
    new_ptr = alloc_pointer + *alloc_pointer / sizeof(unsigned int); //Nastavenie adresy dalsieho bloku, ktory ukazuje na volne miesto po rozdeleni blokov

    //Osetrenie aby som nevybehol z pamate a ci nie je hodnota noveho bloku vacsia ako predosleho
    if (*(char *) new_ptr != OUT_OF_MEMORY && *alloc_pointer < value) {
        *new_ptr = value - *(unsigned int *) alloc_pointer + 1; //Znizenie hodnoty volneho bloku o velkost alokovaneho bloku
    }


    //ODSTRANIT KOMENTAR PRE NASTAVENIE BAJTOV

    //Pomocny ukazovatel, ktory mi ukazuje na zaciatok prvy bajt za hlavickou
    /*char *tmp;
    tmp = (char *)alloc_pointer;
    tmp += sizeof(unsigned int);
    for (int i = 0; i < size; i++){
        *((char *)tmp + i) = ALLOC_VALUE;
    }*/



    return alloc_pointer;
}

/*
 * Funkcia memory_check dostane ukazovatel a ma urcit, ci je dany ukazovatel platny(z inicializovanej pamate) a zatial neuvolneny.
 * Nastavim pomocne ukazovatele na zaciatok a koniec pamate a nasledne porovnam, ci dany ukazovatel je v intervale pamate.
 * V pripade ak nepatri do inicializovanej pamata alebo hlavicka na ktoru ukazuje je volna, funkcia vrati hodnotu 0, inak vrati hodnotu 1.
 */

int memory_check(void *ptr){
    unsigned int* tmp;
    tmp = (unsigned int *)ptr_start - 1;
    unsigned int* max = tmp + *tmp/sizeof(unsigned int) - 1;
    if ((unsigned int *)ptr < tmp || (unsigned int *)ptr > max)
        return 0;
    if (*(unsigned int *)ptr % 2 == 0)
        return 0;
    return 1;
}

/*
 * Funkcia memory_free dostane ukazovatel a ma uvolnit miesto v pamati na adrese, na ktoru ukazuje ukazovatel. Funkcia vrati hodnotu 1 ak sa
 * pamat nepodarilo uvolnit a 0 ak sa podarilo
 */

int memory_free(void *valid_ptr){
    if (valid_ptr == NULL)
        return 1;
    unsigned int* ptr = valid_ptr;
    unsigned int* tmp;
    tmp = (ptr + *ptr/sizeof(unsigned int ));
    static int handler = 1;

    //Ak blok za pozadovanym uvolnovanym je neuvolneny, znizi sa hodnota uvolnovaneho ukazovatela o 1 (tym sa oznaci, ze blok je volny)
    if (*ptr % 2 == 1 && *tmp % 2 == 1){
        *ptr -= 1;
    }
    //Ak blok za pozadovanym uvolnovanym je uvolneny, zvysi sa hodnota uvolnovaneho ukazovatela o velkost volneho bloku za uvolnovanym
    // a odpocita sa 1(indikator prazdneho bloku)
    else if (*ptr % 2 == 1 && *tmp % 2 == 0){
        *ptr = *ptr + *tmp - 1;
    }


    //ODSTRANIT KOMENTAR PRE NASTAVENIE BAJTOV

    //Nastavenie hodnot na zvolenu hodnotu z makra, cyklus ide po velkost volneho bloku
    /*char* help_ptr;
    help_ptr = (char *)(ptr) + sizeof(unsigned int );
    for (int i = 0; i < *ptr - sizeof(unsigned int ); i++){
        if (*(help_ptr + i) == OUT_OF_MEMORY){
            break;
        }
        *(help_ptr + i) = FREE_VALUE;
    }*/


    //Volanie funkcie na spajanie blokov
    coalescing(handler);
    //Zvysenie hodnoty premennej, podla ktorej sa spajaju volne bloky(bloky sa spajaju kazde druhe zavolanie funkcie memory_free
    handler++;

    return 0;
}

/*Zjednocovanie viacerych volnych blokov do jedneho velkeho, ak je 2 a viac volnych blokov vedla seba. Najprv sa nastavia pomocne ukazovatele, ktore
 * vzdy pozeraju na 3 po sebe iduce bloky v pamati. Pomocou while cyklu program prechadza celou pamatou po hlavickach a kontroluje ci su nejake volne
 * bloky vedla seba
 */

void coalescing(int handler){
    unsigned int *ptr;
    unsigned int *tmp;
    char *help_ptr;
    unsigned int *start = (unsigned int *)ptr_start - 1;
    unsigned int *end = start + *start/sizeof(unsigned int) - 1;
    if (handler % 2 == 0){
        unsigned int *prev_ptr = ptr_start;
        ptr = prev_ptr + *prev_ptr/sizeof(unsigned int );
        tmp = (ptr + *ptr/sizeof(unsigned int ));
        while (*tmp != OUT_OF_MEMORY){
            /*
             * Pripad ak su 3 volne bloky v pamati za sebou, k hodnote ukazovatela, ktory je v pamati na najnizsej adrese sa prirataju hodnoty 2 ukazovatelov,
             * ktore su na adresach po nom, potom prebehne cyklus na nastavenie bajtov na nealokovanu hodnotu a ukazovatele sa posunu o blok dalej
             */
            if (*prev_ptr % 2 == 0 && *ptr % 2 == 0 && *tmp %2 == 0){
                *prev_ptr += *ptr + *tmp;



                //ODSTRANIT KOMENTAR PRE NASTAVENIE BAJTOV

                /*help_ptr = (char *)prev_ptr + sizeof(unsigned int );
                for (int i = 0; i < *prev_ptr - sizeof(unsigned int ); i++){
                    if (*(help_ptr + i) == OUT_OF_MEMORY){
                        break;
                    }
                    *(help_ptr + i) = FREE_VALUE;
                }*/


                ptr = prev_ptr + *prev_ptr/sizeof(unsigned int ); //Posun ukazovatela na dalsiu hlavicku
            }
                /*
                 * Pripad ak su volne 2 bloky v pamati za sebou(3. blok je alokovany), tentoraz sa zvysi hodnota ukazovatela na najnizsej adrese o hodnotu ukazovatela
                 * kotry nasleduje za nim
                 */
            else if (*prev_ptr % 2 == 0 && *ptr % 2 == 0 && *tmp % 2 == 1){
                *prev_ptr += *ptr;



                //ODSTRANIT KOMENTAR PRE NASTAVENIE BAJTOV

                /*help_ptr = (char *)prev_ptr + sizeof(unsigned int );
                for (int i = 0; i < *prev_ptr - sizeof(unsigned int ); i++){
                    if (*(help_ptr + i) == OUT_OF_MEMORY){
                        break;
                    }
                    *(help_ptr + i) = FREE_VALUE;
                }*/



                ptr = prev_ptr + *prev_ptr/sizeof(unsigned int );
            }
                /*
                 * Adresy ukazovatelov sa posunu o hodnotu hlavicky, na ktoru ukazovali
                 */
            else{
                prev_ptr = prev_ptr + *prev_ptr/sizeof(unsigned int );
                ptr = prev_ptr + *prev_ptr/sizeof(unsigned int );
                tmp = ptr + *ptr/sizeof(unsigned int );
            }
            //Kontrola, ci program nevybehol z platnej pamate
            if (*ptr == OUT_OF_MEMORY || *tmp == OUT_OF_MEMORY || ptr > end || tmp > end)
                break;
        }
    }
}

/*
 * Funkcia na zaokruhlenie pociatocneho miesta na cislo delitelne 4 (smerom nadol). Postupne sa dekrementuje velkost az dokym nie
 * je delitelna 4
 */
unsigned int size_check_init(unsigned int size){
    if (size % 4 != 0){
        while (size % 4 != 0){
            size--;
        }
    }
    return size;
}


/*
 * Funkcia na zaokruhlenie alokovaneho miesta na cislo delitelne 4 (smerom nahor). Postupne sa inkrementuje pozadovana velkost
 * az dokym nie je delitelna 4
 */
unsigned int size_check_alloc(unsigned int size){
    if (size % 4 != 0){
        while (size % 4 != 0){
            size++;
        }
    }
    return size;
}


/*
 * Funkcia first fit na najdenie prveho vyhovujuceho miesta v pamati na alokaciu. Funkcia dostava ako argumenty velkost, ktoru treba alokovat
 * a ukazovatel. Vo funkcii sa na zaciatku nastavi velkost o 4 bajty vacsia, kvoli hlavicke a ukazovatel sa nastavi na zaciatok pamate
 * Vo while cykle sa porovnava ci je blok alkovany pomocou binarnej operacie &, ktora porovna ci sa posledny bit rovna 1 (co znamena ze blok
 * je alokovany). Ak sa najde nealokovany blok, porovna sa, ci je velkost hlavicky postacujuca na alokaciu velkosti
 * Nasledne sa skontroluje ci je funkcia stale v platnej pamati, ak nie, funkcia vracia NULL, lebo nenasla take miesto. V opacnom pripade sa posunie
 * dalej o velkost hlavicky
 */

void *first_fit(void *ptr, unsigned int size){
    size += sizeof(int );
    ptr = ptr_start;
    unsigned int *end;
    end = ptr_start + *((unsigned int *)ptr_start - 1) - 2*sizeof(unsigned int );
    while ((*((unsigned int *)(ptr)) & 1) == 1 || *(unsigned int *)ptr < size){
        if ((unsigned int *)ptr > end || *((char *)ptr) == OUT_OF_MEMORY || *((char*)(ptr + *((unsigned int *)ptr))) == OUT_OF_MEMORY || *((char*)(ptr + *((unsigned int *)ptr) - 1)) == OUT_OF_MEMORY){
            return NULL;
        }
        if (*((unsigned int *)ptr) % 2 == 1)
            ptr = ptr + (*((unsigned int *)(ptr)) - 1);
        else
            ptr = ptr + (*((unsigned int *)(ptr)));
    }
    return ptr;
}

void memory_tester(char *region, unsigned int min_byte, unsigned int max_byte, unsigned int min_memory, unsigned int max_memory){
    //Generovanie nahodnej velkosti pre pamat podla parametrov
    unsigned int curr_memory = ((rand() % (max_memory - min_memory + 1)) + min_memory);
    //Inicializovanie ukazovatelov
    char *pointer[100000];


    int i = 0;
    //Inicializovanie premennych pomocou ktorych sa rata vyuzitie pamate a % alokovanych blokov
    int total_blocks = 0, allocated_blocks = 0, total_bytes = 0, allocated_bytes = 0;

    //Volanie funkcie memory_init
    memory_init(region, curr_memory);

    /*
     * Cyklus, ktory cyklicky alokuje pamat az pokym uz v pamati nie je miesto na dalsiu alokaciu
     */

    //Do-while cyklus, ktory kontroluje, ci sa este da vygenerovat blok, ktory by sa dal alokovat
    do{
        //Generovanie nahodnej velkosti bloku podla parametrov
        unsigned int random_block_size = ((rand() % (max_byte - min_byte + 1)) + min_byte);
        //Podmienka, ci sa nova vygenerovana velkost bloku da alokovat do pamate, ak nie program vygeneruje novu hodnotu a porovnava znova
        if (total_bytes + random_block_size <= curr_memory) {
            //Inkrementacia poctu blokov a poctu bajtov
            total_blocks++;
            total_bytes += random_block_size;
            pointer[i] = (char *)memory_alloc(random_block_size);
            //Podmienka, ktora kontroluje, ci alokacia bola uspesna, ak bola zvysi sa index ukazovatela, pocet alokovanych blokov,
            // pocet alokovanych bajtov a cyklus pokracuje
            if (pointer[i]){
                allocated_bytes += random_block_size;
                allocated_blocks++;
                i++;
            }
        }
    }while (total_bytes <= curr_memory - min_byte);

    /*
     * Cyklus na uvolnovanie pamate, cyklus iteruje po pocet alokovanych ukazovatelov
     * V cykle sa kontroluje platnost ukazovatela a ak je ukazovatel platny, pamat sa uvolni
     */

    for (int j = 0; j < i; j++){
        if (memory_check(pointer[j])){
            memory_free(pointer[j]);
        }
        else {
            printf("Wrong check\n");
        }
    }

    //Vynulovanie pamate
    memset(ptr_start, 0, curr_memory);

    //Volanie funkcie pre vypocet a vypis % hodnot
    print_percentage(total_bytes, allocated_bytes, total_blocks, allocated_blocks, curr_memory);

}

void print_percentage(int total_bytes, int allocated_bytes, int total_blocks, int allocated_blocks, int curr_memory){
    double block_percentage, byte_percentage, memory_percentage;
    printf("Total memory: %d bytes\n\tTotal blocks: %d, Allocated blocks: %d\n\tTotal bytes: %d, Allocated bytes: %d\n", curr_memory, total_blocks, allocated_blocks,
           total_bytes, allocated_bytes);
    block_percentage = ((double )allocated_blocks / (double )total_blocks) * 100;
    byte_percentage = ((double )allocated_bytes / (double )total_bytes) * 100;
    memory_percentage = ((double )allocated_bytes / ((double )(curr_memory))) * 100;
    printf("\tMemory used: %.2f%% Blocks allocated: %.2f%%, Bytes allocated: %.2f%%\n\n", memory_percentage, block_percentage, byte_percentage);
}

/*
 * Funkcia sluziaca na volanie testovacej funkcie
 * Ako parametre dostava pamat a pocet iteracii, ktore ma urobit
 */

void print_test(char *region, int iteration){

    printf("TEST 1: Testing memory size 50 - 100 bytes, with fixed block size 8 bytes\n");
    for (int i = 0; i < iteration; i++) {
        printf("\t%d: ", i + 1);
        int min_byte = 8;
        int max_byte = 24;
        int small_block = ((rand() % (max_byte - min_byte + 1)) + min_byte);
        memory_tester(region, small_block, small_block, 50, 200);
    }

    printf("TEST 2: Testing memory size 50 - 200 bytes, with random block size 8 - 24 bytes\n");
    for (int i = 0; i < iteration; i++) {
        printf("\t%d: ", i + 1);
        memory_tester(region, 8, 24, 50, 200);
    }

    printf("TEST 3: Testing memory size 1000 - 50000 bytes, with random block size 500 - 5000 bytes\n");
    for (int i = 0; i < iteration; i++) {
        printf("\t%d: ", i + 1);
        memory_tester(region, 500, 5000, 1000, 50000);
    }

    printf("TEST 4: Testing memory size 100000 - 200000 bytes, with random block size 8 - 50000 bytes\n");
    for (int i = 0; i < iteration; i++) {
        printf("\t%d: ", i + 1);
        memory_tester(region, 8, 50000, 100000, 200000);
    }
}