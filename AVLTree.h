#ifndef AVLTREE_H
#define AVLTREE_H

typedef void deleteFunction(void *item);

typedef struct AVLTree_node {
    int id;
    void *item;
    struct AVLTree_node *parent;
    struct AVLTree_node *leftChild;
    struct AVLTree_node *rightChild;
    char balance; 
} AVLTree_node;

typedef struct AVLTree {
    AVLTree_node *root;
    unsigned int length;
} AVLTree;

/* Returns an AVLTree allocated in memory with initalized data */
AVLTree *AVLTree_create();

/* Returns an AVLTree_node allocated in memory with initalized data */
AVLTree_node *AVLTree_nodeCreate(AVLTree_node *parent, int id, void *item);

unsigned int AVLTree_getLength(AVLTree *tree);

unsigned int AVLTree_getDepth(AVLTree *tree);

/* Place the item at the good position in the tree,
 * Returns true if it succeeded, false if the id is already in the table
 */
bool AVLTree_push(AVLTree *tree, int id, void *item);

void *AVLTree_pop(AVLTree *tree, int id);

void AVLTree_balance(AVLTree *tree, AVLTree_node *node);

void AVLTree_LL(AVLTree *tree, AVLTree_node *node);

void AVLTree_RR(AVLTree *tree, AVLTree_node *node);

void AVLTree_LR(AVLTree *tree, AVLTree_node *node);

void AVLTree_RL(AVLTree *tree, AVLTree_node *node);

void AVLTree_debugPrint(AVLTree *tree);

/* Returns the item corresponding to the id, returns NULL if no item is found with the corresponding id */
void *AVLTree_getItem(AVLTree *tree, int id);

/* Returns the item with the closest id (the item must have a lower id), returns NULL if no item is found */
void *AVLTree_getClosestItemLow(AVLTree *tree, int id);

void *AVLTree_getClosestItemHigh(AVLTree *tree, int id);

void AVLTree_delete(AVLTree *tree);

void AVLTree_deleteFull(AVLTree *tree, deleteFunction *deleteItem);

AVLTree *AVLTree_copy(AVLTree *tree);

#endif /* AVLTREE_H */