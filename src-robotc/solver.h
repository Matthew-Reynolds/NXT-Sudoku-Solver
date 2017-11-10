#ifndef SOLVER_H
#define SOLVER_H

#ifndef UTIL_H
#include "util/util.c"
#endif

typedef struct{
	ubyte board[9][9];
} Sudoku;

ubyte getBoxNumber(ubyte row, ubyte col);
ubyte getBoxEntryCol(ubyte box, ubyte entry);
ubyte getBoxEntryRow(ubyte box, ubyte entry);

bool checkValid(const Sudoku & sudoku);
bool checkSolved(const Sudoku & sudoku);
bool checkCol(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkRow(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool checkBox(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool reynoldsSolverInner(short * potentialColVals, short * potentialRowVals, short * potentialBoxVals, Sudoku & sudoku);
bool reynoldsSolver(Sudoku & sudoku);

#endif
