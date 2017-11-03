#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

typedef struct{
	float lastTime;
	float inputVal, output, setpoint;
	float integralTerm;
	float kP, kI;
	float minimumOutput, maximumOutput;
	float minimumInput, maximumInput;

	bool isContinuous;
	float tolerance;

	bool sourceIsDiscreet;
	float * sourcef;
	long * sourcel;
} PIController;

void initializeController(PIController & controller, float newKP, float newKI, float & newSource);
void initializeController(PIController & controller, float newKP, float newKI, long * newSource);

void setTunings(PIController & controller, float newKP, float newKI);
void setOutputRange(PIController & controller, float newMin, float newMax);
void setInputRange(PIController & controller, float newMin, float newMax);
void setDirection(PIController & controller, bool isForwards);
void setSetpoint(PIController & controller, float newSetpoint);

float getInput(PIController & controller);
float getOutput(PIController & c);
bool onTarget(PIController & controller);

#endif

#ifndef MATH_H
#include "math.c"
#endif
