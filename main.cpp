#include <chrono>
#include <fstream>
#include <iostream>

#include "solve.h"

bool is_valid_puzzle(const std::string_view s) {
    if (s.size() != 81) {
        return false;
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

// Pretty print the puzzle to the console, including lines between boxes.
void pretty_print_puzzle(const std::string& puzzle) {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0) {
            std::cout << "+-------+-------+-------+" << std::endl;
        }
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0) {
                std::cout << "| ";
            }
            std::cout << puzzle[i * 9 + j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "+-------+-------+-------+" << std::endl << std::endl;
}

int main(int argc, char** argv) {
    // Read the input file specfied in the arg[1] if it exists.
    // If it doesn't exist, show usage information.
    std::string puzzle;
    if (argc <= 1) {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    // If the first argument is a valid puzzle, then use it. If not, assume
    // that it's a filename and try to read the puzzle from the file.
    if (is_valid_puzzle(argv[1])) {
        puzzle = argv[1];
    } else {
        std::ifstream ifs(argv[1]);
        if (!ifs) {
            std::cerr << "Error: could not open file " << argv[1] << std::endl;
            return 1;
        }
        puzzle = std::string(std::istreambuf_iterator<char>(ifs),
                             std::istreambuf_iterator<char>());
        // Remove any whitespace characters from the puzzle.
        puzzle.erase(std::remove_if(puzzle.begin(),
                                    puzzle.end(),
                                    [](char c) { return std::isspace(c); }),
                     puzzle.end());
        if (!is_valid_puzzle(puzzle)) {
            std::cerr << "Error: input puzzle must be 81 characters long"
                      << " and contain only digits between 0 and 9"
                      << std::endl;
            return 1;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    // Solve the puzzle.
    Solver solver;
    auto [solution, num_guesses] = solver.solve(puzzle);
    auto end = std::chrono::high_resolution_clock::now();

    double time_taken =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();

    time_taken *= 1e-9;

    // Print the solution and the number of guesses made.
    pretty_print_puzzle(solution);
    std::cout << "Number of guesses: " << num_guesses << std::endl;
    std::cout << "Time taken: " << std::fixed << std::setprecision(9)
              << time_taken << " seconds" << std::endl;

    return 0;
}
