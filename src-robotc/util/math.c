#include "math.h"

float fabs(float input){
	if(input < 0)
		input *= -1;
	return input;
}

int abs(int input){
	if(input < 0)
		input *= -1;
	return input;
}

float fmod(float dividend, float divisor){
	int c = dividend/divisor;
	return dividend-(divisor*c);
}
