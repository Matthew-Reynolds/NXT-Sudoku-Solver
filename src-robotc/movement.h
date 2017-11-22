#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "util/color.h"
#include "util/pi_controller.h"

void raisePen();
void lowerPen();

void readPuzzle(Sudoku & sudoku);
void readRow(Sudoku & sudoku, int curLine);

void printPuzzle(const Sudoku & unsolved, const Sudoku & solved);
void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine);
void plotNumber(int value, long delay = 500, float segH = 0.75, float segW = 1);

void homeAxis();
void findFirstCell();
void moveToCell(int row, int col, bool isRead);
void moveToOffset(float x, float y);

#endif
