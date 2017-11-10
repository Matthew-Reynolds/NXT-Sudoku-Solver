#ifndef UTIL_H
#define UTIL_H

//#ifndef MATH_H
//#include "util/math.c"
//#endif

short decToBit(ubyte dec);
//ubyte bitToDec(short bit);
//bool isSingleBit(short bit);

void copyArray(ubyte * source, ubyte * dest, short size);
void copyArray(short * source, short * dest, short size);

#endif
