#include "pi_controller.h"

void initializeController(PIController & controller, float newKP, float newKI, InputType type, int inPort, int outPort){
	setTunings(controller, newKP, newKI);
	controller.inputType = type;
	controller.inputPort = inPort;
	controller.outputPort = outPort;
	controller.tolerance = 2;
	controller.isContinuous = false;
}


// Update P and I vals
void setTunings(PIController & controller, float newKP, float newKI) {
	controller.kP = fabs(newKP);
	controller.kI = fabs(newKI);
}

void setOutputRange(PIController & controller, float newMin, float newMax) {
	if (newMin > newMax)
		return; // If the lower boundary is greater than the upper boundary,
	// return
	controller.minimumOutput = newMin;
	controller.maximumOutput = newMax;
}

void setInputRange(PIController & controller, float newMin, float newMax) {
	if (newMin > newMax)
		return; // If the lower boundary is greater than the upper boundary,
	// return
	controller.minimumInput = newMin;
	controller.minimumOutput = newMax;
	controller.scale = 1;
	setSetpoint(controller, getInput(controller)*1);
}

void setInputRange(PIController & controller, float newMin, float newMax, float scalingFactor) {
	if (newMin > newMax)
		return; // If the lower boundary is greater than the upper boundary,
	// return
	controller.minimumInput = newMin;
	controller.minimumOutput = newMax;
	controller.scale = scalingFactor;
	setSetpoint(controller, getInput(controller)/scalingFactor);
}

void setDirection(PIController & controller, bool isForwards) {
	if (isForwards) {
		controller.kP = fabs(controller.kP);
		controller.kI = fabs(controller.kI);
	}
	else {
		controller.kP = -1.0 * fabs(controller.kP);
		controller.kI = -1.0 * fabs(controller.kI);
	}
}

void setSetpoint(PIController & controller, float newSetpoint) {

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

float getInput(PIController & controller){
	if(controller.inputType == MOTOR_ENCODER)
		controller.inputVal = nMotorEncoder[controller.inputPort];
	else
		controller.inputVal = SensorValue[controller.inputPort];
	return controller.inputVal;
}

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
		if (fabs(error) > (c.maximumInput - c.minimumInput) / 2) {
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

bool onTarget(PIController & controller) {
	getInput(controller);


	//		if (m_isContinuous)
	//			return (Math.abs(m_setpoint - m_inputVal) < m_tolerance / 100
	//					* Math.abs(m_maximumInput - m_minimumInput));
	// return Math.abs(m_setpoint - m_inputVal) < m_tolerance;


	// If the setpoint is in the right spot
	return (fabs(controller.setpoint - controller.inputVal) <= controller.tolerance);
}

void updateController(PIController & controller){
	motor[controller.outputPort] = getOutput(controller);
}
