#ifndef SOLVER_H
#define SOLVER_H

#define BRICK_SOLVER

// Typedef-ing Sudoku as a 2D array is slightly more efficient than wrapping it in a struct
// Also, shorts are faster than bytes because the NXT has a 16bit processor
// and unsigned vs signed makes no difference.
typedef short Sudoku[9][9];


// As this program is somewhat simpler than main.c,
// the 'linker' has been simplified as well.
#ifndef UTIL_H
#include "util/util.c"
#endif

#ifndef BLUETOOTH_H
#include "bluetooth.c"
#endif

short getBoxNumber(short row, short col);
short getBoxEntryCol(short box, short entry);
short getBoxEntryRow(short box, short entry);

bool checkValid(const Sudoku & sudoku);
bool checkSolved(const Sudoku & sudoku);
bool checkCol(short* potentialColVals, short * potentialRowVals, short * potentialBoxVals,
							Sudoku & sudoku);
bool checkRow(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals,
							Sudoku & sudoku);
bool checkBox(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals,
							Sudoku & sudoku);
bool sudokuSolverInner(short * potentialColVals, short * potentialRowVals,
												short * potentialBoxVals, Sudoku & sudoku);
bool sudokuSolver(Sudoku & sudoku);

#endif
