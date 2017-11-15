#ifndef MOVEMENT_H
#define MOVEMENT_H

void raisePen();
void lowerPen();

void readPuzzle(Sudoku & sudoku);
void readRow(Sudoku & sudoku, int curLine);

void printPuzzle(const Sudoku & unsolved, const Sudoku & solved);
void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine);
void plotNumber(int value);

void homeAxis();

#endif
