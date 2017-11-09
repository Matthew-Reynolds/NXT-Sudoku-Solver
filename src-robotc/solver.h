#ifndef SOLVER_H
#define SOLVER_H

#ifndef MATH_H
#include "util/math.c"
#endif

#ifndef UTIL_H
#include "util/util.c"
#endif

typedef struct{
	short board[9][9];
} Sudoku;

short getBoxNumber(short row, short col);
short getBoxEntryCol(short box, short entry);
short getBoxEntryRow(short box, short entry);

bool checkValid(const Sudoku & sudoku);
bool checkSolved(const Sudoku & sudoku);
bool checkCol(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkRow(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkBox(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool reynoldsSolverInner(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool reynoldsSolver(Sudoku & sudoku);

#endif
