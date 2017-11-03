#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

// CHANGE ME
const float TICKS_PER_CM = 360/(2*PI*2.75);

enum InputType{
	MOTOR_ENCODER,
	ULTRASONIC,
	GYRO,
	OTHER
};

typedef struct{
	float lastTime;
	float inputVal, output, setpoint;
	float integralTerm;
	float kP, kI;
	float minimumOutput, maximumOutput;
	float minimumInput, maximumInput;
	float scale;

	bool isContinuous;
	float tolerance;

	InputType inputType;
	int inputPort;
	int outputPort;
} PIController;

void initializeController(PIController & controller, float newKP, float newKI, InputType type, int inPort, int outPort);

void setTunings(PIController & controller, float newKP, float newKI);
void setOutputRange(PIController & controller, float newMin, float newMax);
void setInputRange(PIController & controller, float newMin, float newMax);
void setDirection(PIController & controller, bool isForwards);
void setSetpoint(PIController & controller, float newSetpoint);

float getInput(PIController & controller);
float getOutput(PIController & c);
bool onTarget(PIController & controller);

void updateController(PIController & controller);
#endif

#ifndef MATH_H
#include "math.c"
#endif
