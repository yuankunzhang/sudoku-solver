#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudokucom.h"

struct sudoku {
    char digits[81];
};

struct sudoku *sudoku_new(char digits[81]) {
    int i, j;
    struct sudoku *sudoku;

    sudoku = (struct sudoku *)malloc(sizeof(struct sudoku));
    if (sudoku == NULL) {
        return NULL;
    }

    memcpy(sudoku->digits, digits, 81);
    return sudoku;
}

void sudoku_free(struct sudoku *sudoku) { free(sudoku); }

void sudoku_print(struct sudoku *sudoku) {
    int i;

    for (i = 0; i < 81; i++) {
        printf("%c ", sudoku->digits[i] == '0' ? '.' : sudoku->digits[i]);
        if ((i + 1) % 3 == 0 && (i + 1) % 9 != 0) {
            printf("|");
        }
        if ((i + 1) % 9 == 0) {
            printf("\n");
        }
        if ((i + 1) % 27 == 0 && i < 54) {
            printf("-------------------\n");
        }
    }
    printf("\n");
}

bool sudoku_can_put_digit(struct sudoku *sudoku, char digit, int idx) {
    int row, col, i, j;

    assert(digit >= '1' && digit <= '9');

    row = idx / 9;
    col = idx % 9;

    // check row
    for (i = 0; i < 9; i++) {
        if (i == col) {
            continue;
        }
        if (sudoku->digits[row * 9 + i] == digit) {
            return false;
        }
    }

    // check col
    for (i = 0; i < 9; i++) {
        if (i == row) {
            continue;
        }
        if (sudoku->digits[i * 9 + col] == digit) {
            return false;
        }
    }

    // check subgrid
    row = (row / 3) * 3;
    col = (col / 3) * 3;
    for (i = 0; i < 9; i++) {
        j = (row + i / 3) * 9 + (col + i % 3);
        if (j == idx) {
            continue;
        }
        if (sudoku->digits[j] == digit) {
            return false;
        }
    }

    return true;
}

bool sudoku_solve(struct sudoku *sudoku) {
    int i;
    char digit;

    for (i = 0; i < 81; i++) {
        if (sudoku->digits[i] == '0') {
            break;
        }
    }

    if (i == 81) {
        return true;
    }

    for (digit = '1'; digit <= '9'; digit++) {
        if (sudoku_can_put_digit(sudoku, digit, i)) {
            sudoku->digits[i] = digit;
            if (sudoku_solve(sudoku)) {
                return true;
            }
        }
    }

    sudoku->digits[i] = '0';
    return false;
}

// char *puzzle = "49003200000019640021804069318002906096000000407400082050"
//              "1084206800013900000000030";

int main() {
    struct sudoku *sudoku;
    char puzzle[100];

    if (!get_puzzle(puzzle, "evil")) {
        fprintf(stderr, "can't get puzzle\n");
        exit(1);
    }

    sudoku = sudoku_new(puzzle);
    if (sudoku == NULL) {
        perror("can't create new sudoku\n");
    }

    printf("mission:\n");
    sudoku_print(sudoku);
    if (sudoku_solve(sudoku)) {
        printf("solution:\n");
        sudoku_print(sudoku);
    } else {
        printf("can't find a solution\n");
    }

    get_puzzle(puzzle, "easy");

    sudoku_free(sudoku);
    return 0;
}
