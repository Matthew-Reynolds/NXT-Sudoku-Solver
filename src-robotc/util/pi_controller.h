#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

enum InputType{
	MOTOR_ENCODER,
	ULTRASONIC,
	GYRO,
	OTHER
};

typedef struct{
	float lastTime;
	float inputVal, outputVal, setpoint;
	float integralTerm;
	float kP, kI;
	float minimumOutput, maximumOutput;
	float minimumInput, maximumInput;
	float scale;

	bool isReversed;
	bool isContinuous;
	float tolerance;

	InputType inputType;
	int inputPort;
	int outputPort;
	int onTargetCount;

	bool isEnabled;
} PIController;

void initializeController(PIController & controller, float newKP, float newKI, InputType type, int inPort, int outPort);

void setTunings(PIController & controller, float newKP, float newKI);
void setTolerance(PIController & controller, float newTol);
void setOutputRange(PIController & controller, float newMin, float newMax);
void setInputRange(PIController & controller, float newMin, float newMax, float scalingFactor = 1);
void setReversed(PIController & controller, bool reverse);
void setSetpoint(PIController & controller, float newSetpoint);

float getInput(PIController & controller);
float getOutput(PIController & c);
bool onTarget(PIController & controller);

void updateController(PIController & controller);


const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];

#endif

#ifndef MATH_H
#include "math.c"
#endif
