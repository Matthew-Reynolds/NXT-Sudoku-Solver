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
	controller.isContinuous = false;
	controller.scale = 1;
	controller.isEnabled = false;
}

// Update P and I tunings
void setTunings(PIController & controller, float newKP, float newKI) {
	controller.kP = abs(newKP);
	controller.kI = abs(newKI);
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

// Set the range of the possible input values, with a default scaling factor of 1
void setInputRange(PIController & controller, float newMin, float newMax) {

	// Check that the bounds are legal
	if (newMin < newMax){
		controller.minimumInput = newMin;
		controller.minimumOutput = newMax;
		controller.scale = 1;
		setSetpoint(controller, getInput(controller));
	}
	else {
		controller.minimumInput = 0;
		controller.maximumInput = 0;
	}
}

// Set the range of the possible input values and the scaling factor
void setInputRange(PIController & controller, float newMin, float newMax, float scalingFactor) {

	// Check that the bounds are legal
	if (newMin < newMax){
		controller.minimumInput = newMin;
		controller.minimumOutput = newMax;
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

// Updat the input value
float getInput(PIController & controller){
	if(controller.inputType == MOTOR_ENCODER)
		controller.inputVal = nMotorEncoder[controller.inputPort];
	else
		controller.inputVal = SensorValue[controller.inputPort];
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
	c.integralTerm += c.kI * (error * timeChange);

	// If the controller is cts
	if (c.isContinuous) {
		if (abs(error) > (c.maximumInput - c.minimumInput) / 2) {
			if (error > 0) {
				error = error - c.maximumInput + c.minimumInput;
				} else {
				error = error + c.maximumInput - c.minimumInput;
			}
		}
	}

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

	return output;
}

// Determine whether or not the motor has reached is at the setpoint
bool onTarget(PIController & controller) {
	getInput(controller);

	//		if (m_isContinuous)
	//			return (Math.abs(m_setpoint - m_inputVal) < m_tolerance / 100
	//					* Math.abs(m_maximumInput - m_minimumInput));
	// return Math.abs(m_setpoint - m_inputVal) < m_tolerance;

	// If the setpoint is in the right spot
	return (abs(controller.setpoint - controller.inputVal) <= controller.tolerance);
}

// Update the motor's power
void updateController(PIController & controller){
	motor[controller.outputPort] = getOutput(controller);
}


// =*=*=*=*= Currently not implemented =*=*=*=*=

//void setDirection(PIController & controller, bool isForwards) {
//	if (isForwards) {
//		controller.kP = abs(controller.kP);
//		controller.kI = abs(controller.kI);
//	}
//	else {
//		controller.kP = -1.0 * abs(controller.kP);
//		controller.kI = -1.0 * abs(controller.kI);
//	}
//}
