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
