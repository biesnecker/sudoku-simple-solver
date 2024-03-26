#include "solve.h"

#include <bit>
#include <iostream>

std::tuple<std::string, size_t> Solver::solve(const std::string& puzzle) {
    std::string solution = puzzle;
    init(puzzle);
    solve_backtrack(solution, 0);
    return std::make_tuple(solution, num_guesses_);
}

void Solver::init(const std::string& puzzle) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int idx = r * 9 + c;
            int b = (r / 3) * 3 + c / 3;
            uint32_t n = static_cast<uint32_t>(puzzle[idx] - '0');
            if (n != 0) {
                uint32_t mask = 1u << (n - 1);
                rows_[r] ^= mask;
                cols_[c] ^= mask;
                boxes_[b] ^= mask;
            } else {
                todo_.emplace_back(r, c, b);
            }
        }
    }
}

bool Solver::solve_backtrack(std::string& solution, size_t index) {
    // Move the most constrained remaining todo to the front.
    std::nth_element(todo_.begin() + index,
                     todo_.begin() + index,
                     todo_.end(),
                     [this](const RowColBox& a, const RowColBox& b) {
                         return num_candidates(a) < num_candidates(b);
                     });

    auto [r, c, b] = todo_[index];

    auto candidates = rows_[r] & cols_[c] & boxes_[b];
    while (candidates != 0) {
        uint32_t candidate = 1u << std::countr_zero(candidates);
        if (candidates ^ candidate) {
            num_guesses_++;
        }

        rows_[r] ^= candidate;
        cols_[c] ^= candidate;
        boxes_[b] ^= candidate;

        if (index + 1 == todo_.size() || solve_backtrack(solution, index + 1)) {
            solution[r * 9 + c] = '1' + std::countr_zero(candidate);
            return true;
        }

        // Backtrack.
        rows_[r] |= candidate;
        cols_[c] |= candidate;
        boxes_[b] |= candidate;

        // Unset the LSB of candidates.
        candidates &= (candidates - 1);
    }
    return false;
}

int Solver::num_candidates(const RowColBox& rcb) {
    auto [r, c, b] = rcb;
    return std::popcount(rows_[r] & cols_[c] & boxes_[b]);
}
