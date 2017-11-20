#include "pi_controller.h"

// Pls call me structs don't let us initialize their contents
void initializeController(PIController & controller, float newKP, float newKI, InputType type, int inPort, int outPort){

	// Initialize all the specified vars to their specified values
	setTunings(controller, newKP, newKI);
	controller.inputType = type;
	controller.inputPort = inPort;
	controller.outputPort = outPort;

	// Initialize all of the remaining vars to default vals
	controller.lastTime = time1[T4];
	controller.inputVal = 0;
	controller.outputVal = 0;
	controller.setpoint = 0;
	controller.integralTerm = 0;
	controller.tolerance = 2;
	setInputRange(controller, -1, 1);
	setOutputRange(controller, -1, 1);
	controller.isReversed = false;
	controller.isContinuous = false;
	controller.scale = 1;
	controller.isEnabled = false;
}

// Update P and I tunings
void setTunings(PIController & controller, float newKP, float newKI) {
	controller.kP = abs(newKP);
	controller.kI = abs(newKI);
}

void setTolerance(PIController & controller, float newTol) {
	controller.tolerance = abs(newTol);
}

// Set the range of possible output values
void setOutputRange(PIController & controller, float newMin, float newMax) {

	// Check that the bounds are legal
	if (newMin < newMax){
		controller.minimumOutput = newMin;
		controller.maximumOutput = newMax;
	}
	else {
		controller.minimumOutput = 0;
		controller.maximumOutput = 0;
	}
}

// Set the range of the possible input values and the scaling factor
void setInputRange(PIController & controller, float newMin, float newMax, float scalingFactor) {

	// Check that the bounds are legal
	if (newMin < newMax){
		controller.minimumInput = newMin;
		controller.maximumInput = newMax;
		controller.scale = scalingFactor;
		setSetpoint(controller, getInput(controller)/scalingFactor);
	}
	else {
		controller.minimumInput = 0;
		controller.maximumInput = 0;
	}
}

// Set the setpoint of the controller
void setSetpoint(PIController & controller, float newSetpoint) {
	controller.integralTerm = 0;


	// Scale the setpoint
	newSetpoint *= controller.scale;

	// Make sure that the setPoint is within the input limits


	// If the controller is continous...
	if (controller.isContinuous) {
		float range = (controller.maximumInput - controller.minimumInput);

		// Put the setpoint into the range of the input vals
		if (newSetpoint > controller.maximumInput) {
			controller.setpoint = fmod(newSetpoint, range);
		}
		else if (newSetpoint < controller.minimumInput) {
			while (newSetpoint < controller.minimumInput)
				newSetpoint += range;
			controller.setpoint = newSetpoint;
		}

		// If it's already in range, just assign it
		else {
			controller.setpoint = newSetpoint;
		}
	}


	// If the function is not continuous...
	else {
		if(newSetpoint > controller.maximumInput)
			controller.setpoint = controller.maximumInput;
		else if(newSetpoint < controller.minimumInput)
			controller.setpoint = controller.minimumInput;
		else
			controller.setpoint = newSetpoint;
	}
}

// Update the input value
float getScaledInput(PIController & controller){
	return getInput(controller)/controller.scale;
}

// Update the input value
float getInput(PIController & controller){
	if(controller.inputType == MOTOR_ENCODER)
		controller.inputVal = nMotorEncoder[controller.inputPort];
	else
		controller.inputVal = SensorValue[controller.inputPort];

	if(controller.isReversed)
		controller.inputVal *= -1;

	return controller.inputVal;
}

// Calculate the output value
float getOutput(PIController & c) {
	getInput(c);

	// Calculate the elapsed time since the function was last called
	long now = time1[T4]; // RESERVED FOR CONTROLLERS! DO NOT RESET **********
	long timeChange = (now - c.lastTime);
	c.lastTime = now;

	// Compute all the working error variables
	float error = c.setpoint - c.inputVal;

	// If the controller is cts
	/*if (c.isContinuous) {
		if (abs(error) > (c.maximumInput - c.minimumInput) / 2) {
			if (error > 0) {
				error = error - c.maximumInput + c.minimumInput;
				} else {
				error = error + c.maximumInput - c.minimumInput;
			}
		}
	}*/

	// Only add to the integral term as we get close to the setpoint
	if(c.kP * error < c.maximumOutput/6 && c.kP * error > c.minimumOutput/6)
		c.integralTerm += c.kI * error * timeChange;


	// Limit the integral, to prevent huge buildup after we exceed the
	// maximum output
	if (c.integralTerm > c.maximumOutput)
		c.integralTerm = c.maximumOutput;
	else if (c.integralTerm < c.minimumOutput)
		c.integralTerm = c.minimumOutput;

	// Compute the output, limiting it to the boundaries

	float output = (c.kP * error) + c.integralTerm;
	if (output > c.maximumOutput)
		output = c.maximumOutput;
	else if (output < c.minimumOutput)
		output = c.minimumOutput;

	if(c.isReversed)
		output *= -1;

	return output;
}

// Determine whether or not the motor has reached is at the setpoint
bool onTarget(PIController & controller) {
	getInput(controller);

	if((abs(controller.setpoint - controller.inputVal) <= controller.tolerance))
		controller.onTargetCount++;
	else
		controller.onTargetCount = 0;


	//		if (m_isContinuous)
	//			return (Math.abs(m_setpoint - m_inputVal) < m_tolerance / 100
	//					* Math.abs(m_maximumInput - m_minimumInput));
	// return Math.abs(m_setpoint - m_inputVal) < m_tolerance;

	// If the setpoint is in the right spot
	return controller.onTargetCount > 100;
}

// Update the motor's power
void updateController(PIController & controller){
	motor[controller.outputPort] = getOutput(controller);
}

void setReversed(PIController & controller, bool reverse) {
	controller.isReversed = reverse;
}
