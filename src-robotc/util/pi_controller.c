#include "pi_controller.h"
/**
 *	All functions, Author: Matthew Reynolds
 */


/**
*	Initalize the specified PI controller with the given values
*/
void initializeController(PIController & controller, float newKP, float newKI,
													InputType type, int inPort, int outPort){

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


/**
*	Set the P and I coefficients for the specified controller
*
*	param PIController controller
*			The controller to edit
*
*	param float newKP
*			The new proportional coefficient to set
*
*	param float newI
*			The new integral coefficient to set
*/
void setTunings(PIController & controller, float newKP, float newKI) {
	controller.kP = abs(newKP);
	controller.kI = abs(newKI);
}


/**
*	Set the acceptable tolerance for the controller for it
*	to be considered "On target"
*
*	param PIController controller
*			The controller to edit
*
*	param float newTol
*			The tolerance to set, in the raw sensor units! (Not scaled)
*/
void setTolerance(PIController & controller, float newTol) {
	controller.tolerance = abs(newTol);
}


/**
*	Set whether or not this controller should be reversed
*
*	param PIController controller
*			The controller to edit
*
*	param bool reverse
*			Whether or not to reverse the controller
*/
void setReversed(PIController & controller, bool reverse) {
	controller.isReversed = reverse;
}


/**
*	Set the range of allowable output values. If the
*	minimum value exceeds the maximum, set the range
*	to 0 to protect the output device (No output will
*	be calculated by the controller until a legal
*	range is set)
*
*	param PIController controller
*			The controller to edit
*
*	param float newMin
*			The minimum output value to set
*
*	param float newMax
*			The maximum output value to set
*/
void setOutputRange(PIController & controller, float newMin, float newMax) {

	// Check that the bounds are legal
	if (newMin <= newMax){
		controller.minimumOutput = newMin;
		controller.maximumOutput = newMax;
	}
	else {
		controller.minimumOutput = 0;
		controller.maximumOutput = 0;
	}
}

/**
*	Set the range of allowable input values, as well
*	as the scaling factor to convert this raw value
*	(eg voltage) into a useful unit (eg degrees, cm).
*	If the minimum value exceeds or equals the maximum,
*	set the range to default [-1,1].
*
*	param PIController controller
*			The controller to edit
*
*	param float newMin
*			The minimum input value to set
*
*	param float newMax
*			The maximum input value to set
*
*	param float scalingFactor
*			The scaling conversion between the raw sensor
*			value and the useful units (Default: 1)
*/
void setInputRange(PIController & controller, float newMin, float newMax, float scalingFactor) {

	// Check that the bounds are legal
	if (newMin < newMax){
		controller.minimumInput = newMin;
		controller.maximumInput = newMax;
		controller.scale = scalingFactor;
		setSetpoint(controller, getInput(controller)/scalingFactor);
	}
	else {
		controller.minimumInput = -1;
		controller.maximumInput = 1;
	}
}


/**
*	Set the setpoint of the controller, in scaled
*	units based off of the controller's scaling factor
*	(eg cm rather than volts). If the setpoint exceeds
*	the controller's max or min input value, set the
*	setpoint to the greatest allowable limit.
*
*	param PIController controller
*			The controller to edit
*
*	param float newSetpoint
*			The setpoint to set
*/
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


/**
*	Update the controller's input value, and return
*	the value in scaled units
*
*	param PIController controller
*			The controller to update
*
*	return float
*			The controller's input value, in scaled units
*/
float getScaledInput(PIController & controller){
	return getInput(controller)/controller.scale;
}


/**
*	Update the controller's input value from its
*	specified sensor/input
*
*	param PIController controller
*			The controller to update
*
*	return float
*			The controller's input value
*/
float getInput(PIController & controller){
	if(controller.inputType == MOTOR_ENCODER)
		controller.inputVal = nMotorEncoder[controller.inputPort];
	else
		controller.inputVal = SensorValue[controller.inputPort];

	if(controller.isReversed)
		controller.inputVal *= -1;

	return controller.inputVal;
}


/**
*	Determine if the controller is on target
*
*	param PIController controller
*			The controller to check
*
*	return bool
*			Whether or not the controller is on target
*/
bool onTarget(PIController & controller) {
	getInput(controller);

	float err = abs(controller.setpoint - controller.inputVal);

	// If the controller is cts, get the smallest err
	//	if(controller.isContinuous && err > (maximumInput-minimumInput)/2)
	//		err = maximumInput-err;

	if(err <= controller.tolerance)
		controller.onTargetCount++;
	else
		controller.onTargetCount = 0;


	// If the setpoint is in the right spot
	return controller.onTargetCount > 100;
}


/**
*	Calculate the current effort that should be applied
*	to the controller's plant (eg motor).
*
*	param PIController controller
*			The controller to update
*
*	return float
*			The calculated effort to apply to the plant
*/
float getOutput(PIController & c) {
	getInput(c);

	// Calculate the elapsed time since the function was last called
	long now = time1[T4]; // RESERVED FOR CONTROLLERS! DO NOT RESET **********
	long timeChange = (now - c.lastTime);
	c.lastTime = now;

	// Compute all the working error variables
	float error = c.setpoint - c.inputVal;

	//	// If the controller is cts
	//	if (c.isContinuous) {
	//		if (abs(error) > (c.maximumInput - c.minimumInput) / 2) {
	//			if (error > 0) {
	//				error = error - c.maximumInput + c.minimumInput;
	//				} else {
	//				error = error + c.maximumInput - c.minimumInput;
	//			}
	//		}
	//	}

	// Only add to the integral term as we get close to the setpoint
	if(c.kP * error < c.maximumOutput/6 && c.kP * error > c.minimumOutput/6)
		c.integralTerm += c.kI * error * timeChange;


	// Limit the integral, to prevent huge buildup
	if (c.integralTerm > c.maximumOutput)
		c.integralTerm = c.maximumOutput;
	else if (c.integralTerm < c.minimumOutput)
		c.integralTerm = c.minimumOutput;

	// Compute the output, limiting it to the max/min output
	float output = (c.kP * error) + c.integralTerm;
	if (output > c.maximumOutput)
		output = c.maximumOutput;
	else if (output < c.minimumOutput)
		output = c.minimumOutput;

	if(c.isReversed)
		output *= -1;

	return output;
}


/**
*	Calculate the current effort to apply to the controller's
*	output, and set the motor accordingly
*
*	param PIController controller
*			The controller to update
*/
void updateController(PIController & controller){
	motor[controller.outputPort] = getOutput(controller);
}
