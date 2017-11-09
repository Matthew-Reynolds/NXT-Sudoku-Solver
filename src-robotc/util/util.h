#ifndef UTIL_H
#define UTIL_H

#include "math.c"

short decToBit(short dec);
short bitToDec(short bit);
bool isSingleBit(short bit);

void copyArray(short * source, short * dest, short size);

#endif
