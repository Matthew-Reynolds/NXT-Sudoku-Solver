#include "math.h"

int nabs(int input){
	return round(abs(input));
}

float fmod(float dividend, float divisor){
	int c = dividend/divisor;
	return dividend-(divisor*c);
}
