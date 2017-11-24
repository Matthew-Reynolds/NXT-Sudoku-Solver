#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "main.h"
#include "util/color.h"
#include "util/pi_controller.h"

int getDelay();
void raisePen();
void lowerPen();

void readPuzzle(Sudoku & sudoku);
void readRow(Sudoku & sudoku, int curLine);

void printPuzzle(const Sudoku & unsolved, const Sudoku & solved);
void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine);
void plotNumber(int value, float segH = 0.75, float segW = 1);

void homeAxis();
void moveToCell(int row, int col, bool isRead);
void moveToOffset(float x, float y, long delay = 100);

#endif
