#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main() {
    char puzzle[82] = {0};

    sudoku_generate(puzzle, easy);
    sudoku_print(puzzle);

    // struct sudoku *sudoku;
    // size_t count;
    // char puzzle[81] =
    // "49003200000019640021804069318002906096000000407400082050"
    //                   "1084206800013900000000030";

    // sudoku = sudoku_init(puzzle);
    // if (!sudoku) {
    //     fprintf(stderr, "failed to initialize\n");
    //     exit(1);
    // }

    // printf("puzzle:\n");
    // sudoku_print(sudoku->digits);
    // count = sudoku_solve(sudoku, 1, false);
    // if (count == 0) {
    //     printf("no solution found!\n");
    //     return 0;
    // } else {
    //     printf("solution:\n");
    //     sudoku_print(sudoku->solutions);
    // }

    // sudoku_free(sudoku);
    return 0;
}
