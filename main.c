#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#define MAX_THREADS 64

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -input <input_file> -nbuffer <num_buffers> -dict <dictionary_file> -size <puzzle_size> -len <min_len>:<max_len> [-s]\n", prog_name);
}

int validate_args(int argc, char *argv[], char **input_file, int *num_buffers, char **dict_file, int *puzzle_size, int *min_len, int *max_len, int *print_flag) {
    if (argc < 11 || argc > 12) {
        print_usage(argv[0]);
        return 0;
    }

    *print_flag = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-input") == 0 && i + 1 < argc) {
            *input_file = argv[++i];
        } else if (strcmp(argv[i], "-nbuffer") == 0 && i + 1 < argc) {
            *num_buffers = atoi(argv[++i]);
            if (*num_buffers != 1 && *num_buffers != 4 && *num_buffers != 16 && *num_buffers != 64) {
                fprintf(stderr, "Error: num_buffers must be 1, 4, 16, or 64.\n");
                return 0;
            }
        } else if (strcmp(argv[i], "-dict") == 0 && i + 1 < argc) {
            *dict_file = argv[++i];
        } else if (strcmp(argv[i], "-size") == 0 && i + 1 < argc) {
            *puzzle_size = atoi(argv[++i]);
            if (*puzzle_size < 15 || *puzzle_size > 46340) {
                fprintf(stderr, "Error: puzzle_size must be between 15 and 46340.\n");
                return 0;
            }
        } else if (strcmp(argv[i], "-len") == 0 && i + 1 < argc) {
            if (sscanf(argv[++i], "%d:%d", min_len, max_len) != 2 || *min_len < 3 || *max_len > 14 || *min_len > *max_len) {
                fprintf(stderr, "Error: Invalid min_len:max_len format or values out of range.\n");
                return 0;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            *print_flag = 1;
        } else {
            print_usage(argv[0]);
            return 0;
        }
    }
    return 1;
}

// Function to read the puzzle from a file
char** read_puzzle(int fd, int puzzle_size) {
    char **puzzle = (char **)malloc(puzzle_size * sizeof(char *));
    for (int i = 0; i < puzzle_size; i++) {
        puzzle[i] = (char *)malloc((puzzle_size + 1) * sizeof(char));
        read(fd, puzzle[i], puzzle_size);
        puzzle[i][puzzle_size] = '\0';
        lseek(fd, 1, SEEK_CUR); // Skip newline character
    }
    return puzzle;
}

int main(int argc, char *argv[]) {
    char *input_file = NULL;
    int num_buffers = 0;
    char *dict_file = NULL;
    int puzzle_size = 0;
    int min_len = 0, max_len = 0;
    int print_flag = 0;

    if (!validate_args(argc, argv, &input_file, &num_buffers, &dict_file, &puzzle_size, &min_len, &max_len, &print_flag)) {
        return EXIT_FAILURE;
    }

    int buffer_size = MEM_LIMIT / num_buffers;

    // Open input file
    int input_fd = open(input_file, O_RDONLY);
    if (input_fd < 0) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Open dictionary file
    FILE *dict_fp = fopen(dict_file, "r");
    if (!dict_fp) {
        perror("Error opening dictionary file");
        close(input_fd);
        return EXIT_FAILURE;
    }

    // Initialize the dictionary hashset
    dict = set_init();
    load_dictionary(&dict, dict_fp);
    fclose(dict_fp);

    // Read the puzzle
    char **puzzle = read_puzzle(input_fd, puzzle_size);
    close(input_fd);

    // Initialize the BST and mutex
    root = NULL;
    pthread_mutex_init(&lock, NULL);

    // Create threads
    pthread_t threads[MAX_THREADS];
    int subpuzzle_size = puzzle_size / sqrt(num_buffers);
    for (int i = 0; i < sqrt(num_buffers); i++) {
        for (int j = 0; j < sqrt(num_buffers); j++) {
            char ***sub_puzzle_info = (char ***)malloc(3 * sizeof(char **));
            sub_puzzle_info[0] = puzzle + i * subpuzzle_size;
            sub_puzzle_info[1] = (char **)malloc(sizeof(int));
            sub_puzzle_info[2] = (char **)malloc(sizeof(int));
            *((int *)sub_puzzle_info[1]) = subpuzzle_size;
            *((int *)sub_puzzle_info[2]) = subpuzzle_size;

            pthread_create(&threads[i * (int)sqrt(num_buffers) + j], NULL, solve, (void *)sub_puzzle_info);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < num_buffers; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print words in alphabetical order if the flag is set
    if (print_flag) {
        inorder_print(root);
        printf("\n");
    }

    // Clean up
    for (int i = 0; i < puzzle_size; i++) {
        free(puzzle[i]);
    }
    free(puzzle);
    pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}
