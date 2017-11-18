#ifndef SOLVER_H
#define SOLVER_H

// Typedef-ing Sudoku as a 2D array is slightly more efficient than wrapping it in a struct
typedef short Sudoku[9][9];

#ifndef UTIL_H
#include "util/util.c"
#endif

short getBoxNumber(short row, short col);
short getBoxEntryCol(short box, short entry);
short getBoxEntryRow(short box, short entry);

bool checkValid(const Sudoku & sudoku);
bool checkSolved(const Sudoku & sudoku);
bool checkCol(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkRow(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkBox(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool sudokuSolverInner(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool sudokuSolver(Sudoku & sudoku);

#endif
