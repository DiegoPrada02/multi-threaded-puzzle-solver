# 🧩 Multi-threaded Puzzle Solver

A multi-threaded C program that solves word search puzzles. It divides a puzzle grid into sub-puzzles and uses multiple threads to find words from a dictionary within specified length limits, outputting them in alphabetical order if requested.

## 📋 Features

- **Multi-threading**: Uses a producer/consumer model with customizable buffer sizes and thread count for efficient puzzle-solving.
- **Command-line Arguments**: Flexible arguments to control input/output files, buffer sizes, puzzle dimensions, word length range, and sorting.
- **Dictionary Integration**: Loads words from a dictionary file to validate potential matches within the puzzle.
- **Efficient Memory Usage**: Configurable buffer sizes allow optimal memory allocation for large puzzles.

## 🚀 Getting Started

### Prerequisites

- **C Compiler**: Make sure to have `gcc` or another C compiler installed.
- **POSIX Threads (pthread)**: Required for multi-threading functionality.

### Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/diegoprada02/multi-threaded-puzzle-solver.git
   cd multi-threaded-puzzle-solver

    Compile the code:

    bash

    gcc -o puzzle_solver solver.c -lpthread -lm

🔧 Usage

Run the program with the following command-line arguments:

bash

./puzzle_solver -input <input_file> -nbuffer <num_buffers> -dict <dictionary_file> -size <puzzle_size> -len <min_len>:<max_len> [-s]

Arguments

    -input <input_file>: Path to the puzzle input file.
    -nbuffer <num_buffers>: Number of buffers (1, 4, 16, or 64).
    -dict <dictionary_file>: Path to the dictionary file.
    -size <puzzle_size>: Size of the puzzle grid (between 15 and 46340).
    -len <min_len>:<max_len>: Minimum and maximum length of words to search (3–14 characters).
    -s (optional): Sorts the output words in alphabetical order.

Example

bash

./puzzle_solver -input puzzle.txt -nbuffer 16 -dict words.txt -size 100 -len 3:8 -s

📂 File Structure

    solver.h: Contains function prototypes and macros.
    solver.c: Main program file with multi-threaded puzzle-solving logic.
    Makefile: Builds the program.

📌 Function Overview

    validate_args: Validates and parses command-line arguments.
    read_puzzle: Reads the puzzle from the specified file.
    solve: Thread function to solve sub-puzzles.
    inorder_print: Prints found words in alphabetical order if -s is used.

📈 Performance Considerations

    Optimal buffer sizes (1, 4, 16, 64) should be selected based on puzzle size for memory efficiency.
    Each thread processes a sub-puzzle independently to maximize CPU usage.

🔍 Troubleshooting

    Ensure that buffer sizes and puzzle size follow specified constraints.
    Confirm the input and dictionary files are correctly formatted and accessible.
