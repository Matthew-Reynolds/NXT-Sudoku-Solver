task main()
{

	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;

	// Set the motors to coast mode
	bFloatDuringInactiveMotorPWM = true;


	while(true){
			displayTextLine(0, "Motor A:");
			displayTextLine(1, "%d", nMotorEncoder[motorA]);


			displayTextLine(3, "Motor B:");
			displayTextLine(4, "%d", nMotorEncoder[motorB]);
	}

}
