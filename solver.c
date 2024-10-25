#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

// Define global variables
hashset dict;
pthread_mutex_t lock;
tnode* root;
int min_len, max_len;

// Function implementations...

// Initialize the hashset
hashset set_init() {
    hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
    for (int i = 0; i < BINS; i++)
        h.table[i] = NULL;
    return h;
}

// Hash function for strings
static int hash(char *key, int bins) {
    unsigned hashval = 0;
    for (int i = 0; i < strlen(key); i++)
        hashval = 31 * hashval + key[i];
    return hashval % bins;
}

// Function to rehash the hashset
static void rehash(hashset* h) {
    int next_size = next_prime(2 * h->bins);
    node **oldtable = h->table;
    int old_bins = h->bins;
    h->table = (node **)malloc(next_size * sizeof(node *));
    for (int i = 0; i < next_size; i++)
        h->table[i] = NULL;
    h->size = 0;
    h->bins = next_size;
    for (int i = 0; i < old_bins; i++) {
        for (node *it = oldtable[i]; it; it = it->next) {
            insert(h, it->value);
        }
    }
}

// Function to search for a value in the hashset
int search(hashset h, char *val) {
    int index = hash(val, h.bins);
    for (node *iterator = h.table[index]; iterator; iterator = iterator->next) {
        if (!strcmp(iterator->value, val))
            return 1;
    }
    return 0;
}

// Function to insert a value into the hashset
int insert(hashset *h, char *val) {
    if (search(*h, val))
        return 0;
    if (h->size >= h->bins)
        rehash(h);
    int index = hash(val, h->bins);
    node *new_element = (node *)malloc(sizeof(node));
    new_element->next = h->table[index];
    new_element->value = strdup(val);
    h->table[index] = new_element;
    h->size++;
    return 1;
}

// Function to get the next prime number greater than or equal to min
int next_prime(int min) {
    while (1) {
        int prime = 1;
        for (int div = 2; div <= sqrt(min); div++) {
            if (min % div == 0) {
                prime = 0;
                break;
            }
        }
        if (prime)
            return min;
        else
            min++;
    }
    return min;
}

// Function to allocate a new tree node
tnode* talloc() {
    return (tnode*)malloc(sizeof(tnode));
}

// Function to perform in-order traversal of the BST
void inorder_print(tnode* root) {
    if (root == NULL) return;
    inorder_print(root->left);
    printf("%s\n", root->value);
    inorder_print(root->right);
}

// Function to insert a value into the BST
int bst_insert(tnode** root_p, char* val) {
    if ((*root_p) == NULL) {
        (*root_p) = talloc();
        (*root_p)->value = strdup(val);
        (*root_p)->left = (*root_p)->right = NULL;
        return 1;
    }
    if (!strcmp((*root_p)->value, val))
        return 0;
    if (strcmp((*root_p)->value, val) > 0)
        return bst_insert(&((*root_p)->left), val);
    return bst_insert(&((*root_p)->right), val);
}

// Function to load dictionary into hashset
void load_dictionary(hashset *dict, FILE *dictionary_file) {
    char word[100];
    while (fgets(word, sizeof(word), dictionary_file)) {
        // Remove newline character
        word[strcspn(word, "\n")] = '\0';
        // Convert to lowercase
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        insert(dict, word);
    }
}

void insert_word_into_bst_if_valid(char* word) {
    if (search(dict, word)) {
        pthread_mutex_lock(&lock);
        bst_insert(&root, word);
        pthread_mutex_unlock(&lock);
    }
}

void*  search_words_in_subarray(char **sub_puzzle, int rows, int cols, int min_len, int max_len) {
    char buffer[15]; // Buffer to hold words up to 14 characters long

    // Searching rows (left to right and right to left)
    for (int i = 0; i < rows; i++) {
        for (int len = min_len; len <= max_len; len++) {
            for (int start = 0; start <= cols - len; start++) {
                // Left to right
                strncpy(buffer, &sub_puzzle[i][start], len);
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);

                // Right to left
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[i][start + len - 1 - k];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);
            }
        }
    }

    // Searching columns (top to bottom and bottom to top)
    for (int j = 0; j < cols; j++) {
        for (int len = min_len; len <= max_len; len++) {
            for (int start = 0; start <= rows - len; start++) {
                // Top to bottom
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[start + k][j];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);

                // Bottom to top
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[start + len - 1 - k][j];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);
            }
        }
    }

    // Searching diagonals with slope +1 (from top-left to bottom-right and bottom-right to top-left)
    for (int len = min_len; len <= max_len; len++) {
        // From top-left to bottom-right
        for (int i = 0; i <= rows - len; i++) {
            for (int j = 0; j <= cols - len; j++) {
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[i + k][j + k];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);

                // From bottom-right to top-left
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[i + len - 1 - k][j + len - 1 - k];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);
            }
        }
    }

    // Searching diagonals with slope -1 (from bottom-left to top-right and top-right to bottom-left)
    for (int len = min_len; len <= max_len; len++) {
        // From bottom-left to top-right
        for (int i = len - 1; i < rows; i++) {
            for (int j = 0; j <= cols - len; j++) {
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[i - k][j + k];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);

                // From top-right to bottom-left
                for (int k = 0; k < len; k++) {
                    buffer[k] = sub_puzzle[i - len + 1 + k][j + len - 1 - k];
                }
                buffer[len] = '\0';
                insert_word_into_bst_if_valid(buffer);
            }
        }
    }
}

void* solve(void* arg) {
    // Extract the necessary information from the argument
    char*** sub_puzzle_info = (char***)arg;
    char** sub_puzzle = sub_puzzle_info[0];
    int subpuzzle_rows = *((int*)sub_puzzle_info[1]);
    int subpuzzle_cols = *((int*)sub_puzzle_info[2]);

    fprintf(stderr, "solver thread: solving sub-puzzle of dimensions %d by %d.\n", subpuzzle_rows, subpuzzle_cols);

    // Search for words in the sub-puzzle
    search_words_in_subarray(sub_puzzle, subpuzzle_rows, subpuzzle_cols, min_len, max_len);

    // Free dynamically allocated memory
    free(sub_puzzle_info[1]);
    free(sub_puzzle_info[2]);
    free(sub_puzzle_info);
    pthread_exit(NULL);
}

