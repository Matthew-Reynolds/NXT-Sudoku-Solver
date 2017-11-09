#ifndef UTIL_H
#define UTIL_H

#ifndef MATH_H
#include "util/math.c"
#endif

short decToBit(short dec);
short bitToDec(short bit);
bool isSingleBit(short bit);

void copyArray(short * source, short * dest, short size);

#endif
