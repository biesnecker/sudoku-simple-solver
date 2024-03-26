import sys


def readPuzzle(filename):
    with open(filename, "r") as f:
        puzzle = []
        for line in f:
            puzzle.append([])
            for c in line.strip():
                puzzle[-1].append(int(c))
        return puzzle


def solve(puzzle):
    candidates = {}
    for r in range(9):
        for c in range(9):
            if puzzle[r][c] != 0:
                candidates[(r, c)] = set([puzzle[r][c]])
            else:
                candidates[(r, c)] = set([1, 2, 3, 4, 5, 6, 7, 8, 9])
    while eliminate(puzzle, candidates):
        pass
    for r in range(9):
        for c in range(9):
            if puzzle[r][c] == 0:
                return solve_backtrack(puzzle, candidates)
    return puzzle


def solve_backtrack(puzzle, candidates):

    try_count = 0

    def is_valid(puzzle, r, c, k):
        for c1 in range(9):
            if puzzle[r][c1] == k:
                return False
        for r1 in range(9):
            if puzzle[r1][c] == k:
                return False
        for r1 in range((r // 3) * 3, ((r // 3) * 3) + 3):
            for c1 in range((c // 3) * 3, ((c // 3) * 3) + 3):
                if puzzle[r1][c1] == k:
                    return False
        return True

    def solve_backtrack_impl(puzzle, candidates, r, c):
        nonlocal try_count
        if r == 9:
            return True
        if c == 9:
            return solve_backtrack_impl(puzzle, candidates, r + 1, 0)
        if puzzle[r][c] != 0:
            return solve_backtrack_impl(puzzle, candidates, r, c + 1)
        for candidate in candidates[(r, c)]:
            if not is_valid(puzzle, r, c, candidate):
                continue
            puzzle[r][c] = candidate
            try_count += 1
            if solve_backtrack_impl(puzzle, candidates, r, c + 1):
                return True
            puzzle[r][c] = 0
        return False

    solve_backtrack_impl(puzzle, candidates, 0, 0)
    print(f"Combinations tried: {try_count}")
    return puzzle


def eliminate(puzzle, candidates):
    removed = False
    for r in range(9):
        for c in range(9):
            cs = candidates[(r, c)]
            if len(cs) == 1:
                continue

            # Try to eliminate same row values
            for c1 in range(9):
                if c1 == c:
                    continue
                if puzzle[r][c1] != 0 and puzzle[r][c1] in cs:
                    cs.remove(puzzle[r][c1])
                    removed = True
            if len(cs) == 1:
                puzzle[r][c] = list(cs)[0]
                continue

            # Try to eliminate same col values
            for r1 in range(9):
                if r1 == r:
                    continue
                if puzzle[r1][c] != 0 and puzzle[r1][c] in cs:
                    cs.remove(puzzle[r1][c])
                    removed = True
            if len(cs) == 1:
                puzzle[r][c] = list(cs)[0]
                continue

            # Try to eliminate same subgrid values
            for r1 in range((r // 3) * 3, (r // 3 * 3) + 3):
                for c1 in range((c // 3) * 3, (c // 3 * 3) + 3):
                    if r1 == r and c1 == c:
                        continue
                    if puzzle[r1][c1] != 0 and puzzle[r1][c1] in cs:
                        cs.remove(puzzle[r1][c1])
                        removed = True
            if len(cs) == 1:
                puzzle[r][c] = list(cs)[0]
    return removed


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: python3 ./solve.py [FILENAME]")
        sys.exit(1)
    puzzle = readPuzzle(sys.argv[1])
    print("Initial: ", puzzle)
    solved = solve(puzzle)
    print("Solved?: ", solved)
