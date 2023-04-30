#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "AVLTree.h"

/* TODO LIST:
 * - update nodes' balance
 * - develop function to rebalance (this function will decide which balance type to apply)
 * - develop each rebalance type individually
 * - rename functions for a more classical push pop style
 * - overload `[]` operator for a more convenient usage
 */

AVLTree *AVLTree_create() {
    AVLTree *tree = (AVLTree*) malloc(sizeof(AVLTree));
    tree->root = NULL;
    tree->length = 0;
}

AVLTree_node *AVLTree_nodeCreate(AVLTree_node *parent, unsigned int id, void *item) {
    AVLTree_node *node = (AVLTree_node *) malloc(sizeof(AVLTree_node));
    node->id = id;
    node->parent = parent;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->balance = 0;
    node->item = item;
    return node;
}

unsigned int AVLTree_getLength(AVLTree *tree) {
    return tree->length;
}

bool AVLTree_push(AVLTree *tree, unsigned int id, void *item) {
    //printf("----- IN INSERT\n");
    AVLTree_node *node = tree->root;
    if (node == NULL) {
        // printf("A\n");
        /* First node, the new node is then the tree's root */
        tree->root = AVLTree_nodeCreate(NULL, id, item);
        tree->length = 1;
        return true;
        // printf("B\n");
    } else {
        // printf("C\n");
        /* Search the leaf where the item should be inserted */
        AVLTree_node *childNode = node;
        while ((childNode != NULL) && (childNode->id != id)) {
            // printf("D\n");
            node = childNode;
            if (node->id > id) {
                childNode = node->leftChild;
            } else {
                childNode = node->rightChild;
            }
            // printf("E\n");
        }
        // printf("F\n");

        if (childNode != NULL) {
            /* The id was found in the tree, then do nothing and return false to inform failure */
            return false;
        } else {
            // printf("G\n");
            /* Place the node here */
            if (node->id > id) {
                // printf("H\n");
                node->leftChild = AVLTree_nodeCreate(node, id, item);
                node->balance -= 1;
            } else {
                // printf("I\n");
                node->rightChild = AVLTree_nodeCreate(node, id, item);
                node->balance += 1;
            }
            tree->length += 1;
            /* Propagate the balance factor until the first unbalanced (balance in {-2,2}) or perfectly balanced (balance=0) node */
            AVLTree_node *nodeParent = node->parent;
            // printf("J\n");
            while ((nodeParent != NULL) && ((node->balance == -1) || (node->balance == 1))) {
                // printf("K\n");
                if (nodeParent->leftChild == node) {
                    // printf("L\n");
                    nodeParent->balance -= 1;
                } else {
                    // printf("M\n");
                    nodeParent->balance += 1;
                }
                // printf("N\n");
                node = nodeParent;
                nodeParent = node->parent;
                // printf("O\n");
            }
            
            /* Balance the tree if needed */
            // printf("AVANT balance\n");
            AVLTree_balance(tree, node);
            // printf("APRES balance\n");
        }
    }

    //printf("----- OUT INSERT\n");

    return true;
}
#if 0
void *AVLTree_pop(AVLTree *tree, unsigned int id) {
    AVLTree_node *node = tree->root;
    while ((node != NULL) && (node->id != id)) {
        if (node->id < id) {
            node = node->leftChild;
        } else {
            node = node->rightChild;
        }
    }

    if (node != NULL) {
        if (node->parent == NULL) {

        }
        return node->item;
    } else {
        return NULL;
    }
}
#endif

void AVLTree_balance(AVLTree *tree, AVLTree_node *node) {
    //printf("----- IN BALANCE\n");
    if (node->balance == 2) {
        // printf("A\n");
        if ((node->rightChild)->balance == -1) {
            // printf("B\n");
            AVLTree_LR(tree, node);
        } else {
            // printf("C\n");
            AVLTree_LL(tree, node);
        }
    } else if (node->balance == -2) {
        // printf("D\n");
        if ((node->leftChild)->balance==1) {
            // printf("E\n");
            AVLTree_RL(tree, node);
        } else {
            // printf("F\n");
            AVLTree_RR(tree, node);
        }
    }
    //printf("----- OUT BALANCE\n");
}

void AVLTree_LL(AVLTree *tree, AVLTree_node *node) {
    //printf("----- IN LL\n");

    AVLTree_node *parent = node->parent;
    AVLTree_node *rightChild = node->rightChild;

    rightChild->parent = parent;
    if (parent != NULL) {
        if (parent->rightChild == node) {
            parent->rightChild = rightChild;
        } else {
            parent->leftChild = rightChild;
        }
    } else {
        tree->root = rightChild;
    }

    node->rightChild = rightChild->leftChild;
    if (node->rightChild != NULL) {
        (node->rightChild)->parent = node;
    }

    node->parent = rightChild;
    rightChild->leftChild = node;

    if (rightChild->balance == 0) {
        node->balance = 1;
        rightChild->balance = -1;
    } else {
        node->balance = 0;
        rightChild->balance = 0;
    }

    //printf("----- OUT LL\n");
}

void AVLTree_RR(AVLTree *tree, AVLTree_node *node) {
    // printf("----- IN RR\n");

    AVLTree_node *parent = node->parent;
    AVLTree_node *leftChild = node->leftChild;

    leftChild->parent = parent;
    if (parent != NULL) {
        if (parent->rightChild == node) {
            parent->rightChild = leftChild;
        } else {
            parent->leftChild = leftChild;
        }
    } else {
        tree->root = leftChild;
    }

    node->leftChild = leftChild->rightChild;
    if (node->leftChild != NULL) {
        (node->leftChild)->parent = node;
    }

    node->parent = leftChild;
    leftChild->rightChild = node;

    node->balance = 0;
    leftChild->balance = 0;

    // printf("----- OUT RR\n");
}

void AVLTree_LR(AVLTree *tree, AVLTree_node *node) {
    // printf("-----LR\n");
    AVLTree_node *parent = node->parent;
    AVLTree_node *rightChild = node->rightChild;
    AVLTree_node *grandChild = rightChild->leftChild;

    grandChild->parent = parent;
    if (parent != NULL) {
        if (parent->rightChild == node) {
            parent->rightChild = grandChild;
        } else {
            parent->leftChild = grandChild;
        }
    } else {
        tree->root = grandChild;
    }

    node->rightChild = grandChild->leftChild;
    if (grandChild->leftChild != NULL) {
        (grandChild->leftChild)->parent = node;
    }

    rightChild->leftChild = grandChild->rightChild;
    if (rightChild->leftChild != NULL) {
        (rightChild->leftChild)->parent = rightChild;
    }

    grandChild->leftChild = node;
    node->parent = grandChild;

    grandChild->rightChild = rightChild;
    rightChild->parent = grandChild;

    if (grandChild->balance == 0) {
        rightChild->balance = 0;
        node->balance = 0;
    } else if (grandChild->balance > 0) {
        node->balance = -1;
        rightChild->balance = 0;
    } else {
        node->balance = 0;
        rightChild->balance = 1;
    }
    grandChild->balance = 0;
}

void AVLTree_RL(AVLTree *tree, AVLTree_node *node) {
    // printf("----- RL\n");
    AVLTree_node *parent = node->parent;
    AVLTree_node *leftChild = node->leftChild;
    AVLTree_node *grandChild = leftChild->rightChild;

    grandChild->parent = parent;
    if (parent != NULL) {
        if (parent->rightChild == node) {
            parent->rightChild = grandChild;
        } else {
            parent->leftChild = grandChild;
        }
    } else {
        tree->root = grandChild;
    }

    node->leftChild = grandChild->rightChild;
    if (grandChild->rightChild != NULL) {
        (grandChild->rightChild)->parent = node;
    }

    leftChild->rightChild = grandChild->leftChild;
    if (leftChild->rightChild != NULL) {
        (leftChild->rightChild)->parent = leftChild;
    }

    grandChild->rightChild = node;
    node->parent = grandChild;

    grandChild->leftChild = leftChild;
    leftChild->parent = grandChild;

    if (grandChild->balance == 0) {
        leftChild->balance = 0;
        node->balance = 0;
    } else if (grandChild->balance < 0) {
        node->balance = 1;
        leftChild->balance = 0;
    } else {
        node->balance = 0;
        leftChild->balance = -1;
    }
    grandChild->balance = 0;
}

void *AVLTree_getItem(AVLTree *tree, unsigned int id) {
    AVLTree_node *node = tree->root;
    while ((node != NULL) && (node->id != id)) {
        if (node->id > id) {
            node = node->leftChild;
        } else {
            node = node->rightChild;
        }
    }
    return node->item;
}

void AVLTree_debugPrint(AVLTree *tree) {
    printf("length=%d\n", tree->length);
    AVLTree_node *node = tree->root;

    char trace[1000];
    for (int i = 0; i < 1000; i += 1) {
        trace[i] = 0;
    }
    int depth = 0;

    while (depth >= 0) {
        if (trace[depth] == 0) {
            printf("(id=%d\titem=%d\tbalance=%d)\n", node->id, *(int*)node->item, node->balance);
        }
        if (trace[depth] == 0) {
            printf("LEFT: ");
            if (node->leftChild != NULL) {
                trace[depth] = 1;
                node = node->leftChild;
                depth += 1;
            } else {
                printf("NULL\n");
                printf("RIGHT: ");
                if (node->rightChild == NULL) {
                    printf("NULL\n");
                    printf("UP\n");
                    trace[depth] = 0;
                    depth -= 1;
                    node = node->parent;
                } else {
                    trace[depth] = 2;
                    node = node->rightChild;
                    depth += 1;
                }
            }
        } else if (trace[depth] == 1) {
            printf("RIGHT: ");
            if (node->rightChild == NULL) {
                printf("NULL\n");
                printf("UP\n");
                trace[depth] = 0;
                depth -= 1;
                node = node->parent;
            } else {
                trace[depth] = 2;
                node = node->rightChild;
                depth += 1;
            }
        } else {
            printf("UP\n");
            trace[depth] = 0;
            depth -= 1;
            node = node->parent;
        }
    }
}

void AVLTree_delete(AVLTree *tree) {

}

void AVLTree_deleteFull(AVLTree *tree, deleteFunction *deleteItem) {
    int *test = (int *) malloc(sizeof(int));
    *test = 42;
    deleteItem((void*) test);
}

void printTest(void *item) {
    printf("COCORICO MA DOUCE NONO %d\n", *(int*) item);
}

int main() {
    AVLTree *tree = AVLTree_create();

    for (int i = 0; i <= 30; i = i + 1) {
        int *test = (int *) malloc(sizeof(int));
        *test = rand() % 30;
        printf("%d: %d\n", i, *test);
        if (!AVLTree_push(tree, *test, test)) {
            printf("La clé %d est déjà dans l'arbre.\n", *test);
        }
        // AVLTree_debugPrint(tree);
        // getchar();
    }

    AVLTree_debugPrint(tree);

    //AVLTree_deleteFull(tree, printTest);

    return 0;
}