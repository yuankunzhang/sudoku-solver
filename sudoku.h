#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stdbool.h>
#include <stdio.h>

struct sudoku {
    char digits[81];
    char *solutions;
    size_t s_count;
};

enum difficulty {
    easy,
    medium,
    hard,
    expert,
    evil,
};

struct sudoku *sudoku_init(char puzzle[81]);
void sudoku_free(struct sudoku *sudoku);
void sudoku_print(char digits[81]);
void sudoku_generate(char puzzle[81], enum difficulty level);
size_t sudoku_solve(struct sudoku *sudoku, size_t limit, bool random);

#endif // SUDOKU_H_
