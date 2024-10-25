#ifndef SOLVER_H
#define SOLVER_H

#include <pthread.h>
#include <stdio.h> // Include stdio.h for FILE
#include <stdlib.h>
#include <string.h>

// Define constants
#define MEM_LIMIT 64000000
#define BINS 101

// Define structures
struct node {
    struct node *next;
    char *value;
};
typedef struct node node;

typedef struct {
    node **table;
    int size;
    int bins;
} hashset;

struct tnode {
    char* value;
    struct tnode* left;
    struct tnode* right;
};
typedef struct tnode tnode;

// Declare global variables
extern hashset dict;
extern pthread_mutex_t lock;
extern tnode* root;
extern int min_len, max_len;

// Function declarations
void* solve(void* arg);
void print_buffer(char** sub_puzzle, int subpuzzle_rows, int subpuzzle_cols);
int search(hashset h, char *val);
int insert(hashset *h, char *val);
hashset set_init();
tnode* talloc();
void inorder_print(tnode* root);
int bst_insert(tnode** root_p, char* val);
void load_dictionary(hashset *dict, FILE *dictionary_file);
int next_prime(int min);

#endif // SOLVER_H
