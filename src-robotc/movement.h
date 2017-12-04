#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "main.h"
#include "util/color.h"
#include "util/pi_controller.h"

// Height/width of the square cells (cm)
const int CELL_SIZE = 2;

// Offset from the origin to cell (8,8) (cm)
// Note that this is the offset for the read head
const float X_ORIGIN_OFFSET = 1.25;
const float Y_ORIGIN_OFFSET = 5.25;

// Offset between the read and write head (cm)
const float X_WRITE_OFFSET = -0.5;
const float Y_WRITE_OFFSET = -4.75;

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
