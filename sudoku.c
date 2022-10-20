#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sudoku.h"

static char PUZZLE0[81] = "0000000000000000000000000000000000000000000000000000"
                          "00000000000000000000000000000";

static void _generate(char puzzle[81], enum difficulty level, jmp_buf jmpbuf);
static void _solve(struct sudoku *sudoku, size_t limit, bool random,
                   jmp_buf jmpbuf);
static bool _test_digit_at_location(struct sudoku *sudoku, char digit,
                                    size_t loc);
static int _randint(int lower, int upper);
static void _shuffle(char *digits, size_t n);

struct sudoku *sudoku_init(char puzzle[81]) {
    struct sudoku *sudoku = (struct sudoku *)malloc(sizeof(struct sudoku));
    if (!sudoku) {
        perror("failed to allocate memory");
        return NULL;
    }

    srand48(time(NULL));

    memcpy(sudoku->digits, puzzle, 81);
    sudoku->solutions = NULL;
    sudoku->s_count = 0;

    return sudoku;
}

void sudoku_free(struct sudoku *sudoku) {
    if (sudoku->solutions)
        free(sudoku->solutions);
    free(sudoku);
}

void sudoku_print(char digits[81]) {
    for (size_t i = 0; i < 81; i++) {
        printf("%c ", digits[i] == '0' ? '.' : digits[i]);
        if (i % 3 == 2 && i % 9 != 8)
            printf("|");
        if (i % 9 == 8)
            printf("\n");
        if (i % 27 == 26 && i < 54)
            printf("-------------------\n");
    }
    printf("\n");
}

size_t sudoku_solve(struct sudoku *sudoku, size_t limit, bool random) {
    jmp_buf jmpbuf;

    if (setjmp(jmpbuf) != 0) {
        return sudoku->s_count;
    }

    _solve(sudoku, limit, random, jmpbuf);
    return 0;
}

void sudoku_generate(char puzzle[81], enum difficulty level) {
    struct sudoku *sudoku = sudoku_init(PUZZLE0);
    if (!sudoku) {
        fprintf(stderr, "failed to initialize sudoku\n");
        return;
    }

    if (sudoku_solve(sudoku, 1, true) != 1) {
        fprintf(stderr, "failed to generate new puzzle\n");
        return;
    }

    memcpy(puzzle, sudoku->solutions, 81);
    free(sudoku);

    jmp_buf jmpbuf;
    if (setjmp(jmpbuf) != 0) {
        return;
    }

    _generate(puzzle, level, jmpbuf);
}

static void _generate(char puzzle[81], enum difficulty level, jmp_buf jmpbuf) {
    while (true) {
        size_t loc = _randint(0, 81);
        if (puzzle[loc] == '0')
            continue;

        char c = puzzle[loc];
        puzzle[loc] = '0';
        struct sudoku *sudoku = sudoku_init(puzzle);
        if (sudoku_solve(sudoku, 2, false) > 1) {
            puzzle[loc] = c;
            longjmp(jmpbuf, 1);
        }
        free(sudoku);
    }
}

static void _record_solution(struct sudoku *sudoku) {
    sudoku->solutions = realloc(sudoku->solutions, (++sudoku->s_count) * 81);
    memcpy(sudoku->solutions + (sudoku->s_count - 1), sudoku->digits, 81);
}

static void _solve(struct sudoku *sudoku, size_t limit, bool random,
                   jmp_buf jmpbuf) {
    size_t loc;
    char digits[9] = "123456789";

    for (loc = 0; loc < 81; loc++) {
        if (sudoku->digits[loc] == '0')
            break;
    }

    if (loc == 81) {
        _record_solution(sudoku);
        if (sudoku->s_count == limit)
            longjmp(jmpbuf, 1);
        else
            return;
    }

    if (random) {
        _shuffle(digits, sizeof(digits));
    }

    for (size_t j = 0; j < sizeof(digits); j++) {
        if (_test_digit_at_location(sudoku, digits[j], loc)) {
            sudoku->digits[loc] = digits[j];
            _solve(sudoku, limit, random, jmpbuf);
        }
    }

    sudoku->digits[loc] = '0';
}

static bool _test_digit_at_location(struct sudoku *sudoku, char digit,
                                    size_t loc) {
    assert(digit >= '1' && digit <= '9');

    size_t row = loc / 9;
    size_t col = loc % 9;
    size_t i, j;

    // check row
    for (i = 0; i < 9; i++) {
        if (i == col)
            continue;
        if (sudoku->digits[row * 9 + i] == digit)
            return false;
    }

    // check col
    for (i = 0; i < 9; i++) {
        if (i == row)
            continue;
        if (sudoku->digits[i * 9 + col] == digit)
            return false;
    }

    // check square
    row = (row / 3) * 3;
    col = (col / 3) * 3;
    for (i = 0; i < 9; i++) {
        j = (row + i / 3) * 9 + (col + i % 3);
        if (j == loc)
            continue;
        if (sudoku->digits[j] == digit)
            return false;
    }

    return true;
}

static int _randint(int lower, int upper) {
    return (int)(lower + drand48() * (upper - lower));
}

static void _shuffle(char *digits, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        size_t j = _randint(i, n);
        char c = digits[i];
        digits[i] = digits[j];
        digits[j] = c;
    }
}
