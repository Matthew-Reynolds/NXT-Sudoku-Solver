#ifndef UTIL_H
#define UTIL_H

#include "solver.h"
#include "util/math.h"

short decToBit(short dec);
//short bitToDec(short bit);
//bool isSingleBit(short bit);


void copySudoku(const Sudoku & source, Sudoku & dest);
void copyArray(short * source, short * dest, short size);

#endif
