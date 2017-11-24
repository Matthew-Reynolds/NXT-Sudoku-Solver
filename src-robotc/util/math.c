#include "math.h"

/**
 *	Floating-point modulous
 *
 *	Calculate the remainder of floating point division. Note
 *	that, like the % operator, this is not a mathematically
 *	correct modulous, but rather a remainder. The sign of the
 *	remainder is determined solely by the sign of the dividend
 *
 *	param float dividend
 *			The number to be divided
 *
 *	param float divisor
 *			The number to divide by
 *
 *	return float
 *			The calculated remainder
 */
float fmod(float dividend, float divisor){
	int c = dividend/divisor;
	return dividend-(divisor*c);
}
