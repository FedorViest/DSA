
/*
 * Pouzite makra
 */
#define MIN_HEIGHT 1    //Najmensia mozna vyska
#define MIN_DIFFERENCE 1    //Najvacsi pripustny rozdiel vysok medzi 2 uzlami stromu

#define MAX(first, second) (((first) >= (second)) ? (first) : (second))     //Makro na najdenie vacsieho cisla z 2 cisel
#define GET_HEIGHT(root) (((root) == NULL) ? 0 : (root->height))        //Makro na zistenie vysky uzlu, ak uzol neexistuje, vrati NULL
#define HEIGHT_DIFFERENCE(root) ((root) == NULL) ? 0 : ((GET_HEIGHT(root->left)) - (GET_HEIGHT(root->right)))   //Zistovanie rozdielu vysok z 2 uzlov



/*
 * Struktura pre BVS, ktora v sebe uchovava hodnotu, vysku pocet duplikatov hodnoty, smernik na left child, smernik na right child
 */
typedef struct tree{
    int data;
    int height;
    int count;
    struct tree *left;
    struct tree *right;
}TREE;

/*
 * Vsetky funkcie pouzite v programe
 */

TREE *create_node(int data);
TREE *AVL_insert(TREE *root, int data);
TREE *rotate_left(TREE *root);
TREE *rotate_right(TREE *root);
TREE *rotate_LeftRight(TREE *root);
TREE *rotate_RightLeft(TREE *root);
int update_height(TREE *root);
TREE *balance_tree(TREE *root, int data);
TREE *AVL_search(TREE *root, int data);
void preOrder(TREE *root);
void print_rotations();

/*int max(int a, int b);
int height(TREE *root);
int difference(TREE *root);*/
