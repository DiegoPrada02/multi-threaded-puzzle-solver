# 🔐 Multi-threaded Password Generator

This project is a multi-threaded C program that generates passwords by combining words from a dictionary file. It utilizes both producer and consumer threads to populate and access a hashset of words for password creation. 

## 📋 Features

- **Producer/Consumer Model**: Producer threads insert words into a hashset, and consumer threads generate passwords by randomly selecting and deleting words from the hashset.
- **Randomized Password Creation**: Each password is created by selecting a specified number of random words from the dictionary.
- **Multi-threading Support**: The number of producer and consumer threads can be customized, allowing for optimized performance.
- **Concurrency**: Uses mutex locks to ensure safe access to the shared hashset across threads.

## 🚀 Getting Started

### Prerequisites

- **C Compiler**: Ensure you have `gcc` or another C compiler installed.
- **POSIX Threads (pthread)**: Required for multi-threading.

### Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/diegoprada02/multi-threaded-password-generator.git
   cd multi-threaded-password-generator

    Compile the code:

    bash

    gcc -o password_generator main.c hash.c -lpthread -lm

🔧 Usage

Run the program with the following command-line arguments:

bash

./password_generator -p <num_producers> -c <num_consumers> -n <num_passwords> -l <words_per_password> -f <dictionary_file>

Arguments

    -p <num_producers>: Number of producer threads to populate the hashset with words.
    -c <num_consumers>: Number of consumer threads to generate passwords.
    -n <num_passwords>: Total number of passwords to generate.
    -l <words_per_password>: Number of words in each password.
    -f <dictionary_file>: Path to the dictionary file containing words for password generation.

Example

bash

./password_generator -p 4 -c 2 -n 10 -l 3 -f dictionary.txt

This example will use:

    4 producers to load words into the hashset.
    2 consumers to generate 10 passwords, each containing 3 words, using dictionary.txt as the word source.

📂 File Structure

    main.c: The main file with producer and consumer thread logic.
    hash.c: Contains hashset manipulation functions (insert, delete).
    hash.h: Header file for the hashset functions.

📌 Program Flow

    Producer Threads: Each producer inserts a portion of the dictionary words into the shared hashset.
    Consumer Threads: Consumers generate passwords by randomly selecting words from the hashset. Each word selected is removed from the hashset to prevent reuse in other passwords.
    Mutex Locks: Protects the hashset to ensure only one thread accesses it at a time.

📈 Performance Tips

    The number of producer and consumer threads can be adjusted based on the system’s core count for optimal performance.
    Ensure password count (k) is divisible by the number of consumers (m) to avoid errors.
