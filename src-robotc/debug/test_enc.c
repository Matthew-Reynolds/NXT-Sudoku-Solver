/**
 *	This is a simple test program used to check the motor
 *	encoder readings and ensure they align properly with
 *	the predicted values
 *
 *	This program simply continually outputs both motorA and
 *	motorB encoder values.
 *
 *	Author: Matthew Reynolds
 */
task main()
{

	// Reset the encs
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;

	// Set the motors to coast mode
	bFloatDuringInactiveMotorPWM = true;

	// Loop until a button is pressed
	while(nNxtButtonPressed == -1){
			displayTextLine(0, "Motor A:");
			displayTextLine(1, "%d", nMotorEncoder[motorA]);

			displayTextLine(3, "Motor B:");
			displayTextLine(4, "%d", nMotorEncoder[motorB]);
	}

}
