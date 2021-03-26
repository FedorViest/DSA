
/*
 * Pouzite makra
 */

#define MIN_SIZE 10 //najmensia mozna velkost tabulky
#define FREE_VALUE 0    //oznacenie prazdneho miesta v tabulke
#define OCCUPIED_VALUE 1    //oznacenie zaplneneho miesta v tabulke
#define MAX_OCCUPIED 0.75   //najvacsia mozna pripustnost zaplnenia tabulky

/*
 * Struktura, ktora v sebe uchovava informaciu o hodnote a zaplneni tabulky
 */
typedef struct unit{
    int data;
    int full;
}UNIT;

/*
 * Vsetky funkcie pouzite v programe
 */
UNIT *init_LP_hash(UNIT *hash_table);
int LP_create_hash(int data);
UNIT *LP_hash_insert(UNIT *hash_table, int data);
UNIT *resize_table(UNIT *hash_table);
UNIT *LP_hash_search(UNIT *hash_table, int data);
int even_size();

