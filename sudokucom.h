#ifndef SUDOKUCOM_H_
#define SUDOKUCOM_H_

#include <stdbool.h>

// Get a puzzle from sudoku.com. The `puzzle` argument must have space
// for 81 + 1 characters (the extra 1 byte is for the ending null byte).
bool get_puzzle(char *puzzle, const char *difficulty);

#endif // SUDOKUCOM_H_
