#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

constexpr uint32_t kAll = 0x1FF;
typedef std::tuple<int, int, int> RowColBox;

class Solver {
  public:
    Solver() : rows_{}, cols_{}, boxes_{}, todo_{}, num_guesses_{0} {
        rows_.fill(kAll);
        cols_.fill(kAll);
        boxes_.fill(kAll);
    }

    std::tuple<std::string, size_t> solve(const std::string& puzzle);

  private:
    std::array<uint32_t, 9> rows_, cols_, boxes_;
    std::vector<RowColBox> todo_;
    size_t num_guesses_;

    void init(const std::string& puzzle);
    bool solve_backtrack(std::string& solution, size_t index);
    int num_candidates(const RowColBox& rcb);
};
