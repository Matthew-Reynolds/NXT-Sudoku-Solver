#include "util.h"

short decToBit(short dec){
	return (short) pow(2, dec-1);
}

short bitToDec(short bit){
	if(bit < 1)
		return 0;

	return (short)(log(bit)/log(2)) + 1;
}

bool isSingleBit(short bit){
	if(bit < 1)
		return false;
	return fmod(log(bit)/log(2), 1) < 0.01;
}

void copyArray(short * source, short * dest, short size){
	for(int cur = 0; cur < size; cur++){
		dest[cur] = source[cur];
	}
}
