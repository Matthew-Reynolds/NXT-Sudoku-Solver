#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "util/color.h"

void raisePen();
void lowerPen();

// Implement these two
void readPuzzle(Sudoku & sudoku);
void readRow(Sudoku & sudoku, int curLine);


void printPuzzle(const Sudoku & unsolved, const Sudoku & solved);
void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine);
void plotNumber(int value);

void homeAxis();
void findFirstCell();
void moveToCell(int row, int col);

#endif
