#include "util.h"

short decToBit(ubyte dec){
	return (short) pow(2, dec-1);
}

//ubyte bitToDec(short bit){
//	if(bit < 1)
//		return 0;
//
//	return (ubyte)(log(bit)/log(2)) + 1;
//}
//
//bool isSingleBit(short bit){
//	if(bit < 1)
//		return false;
//	return fmod(log(bit)/log(2), 1) < 0.01;
//}

void copyArray(ubyte * source, ubyte * dest, short size){
	for(short cur = 0; cur < size; cur++){
		dest[cur] = source[cur];
	}
}

void copyArray(short * source, short * dest, short size){
	for(short cur = 0; cur < size; cur++){
		dest[cur] = source[cur];
	}
}
