#include "main.h"

const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];
RobotState state = DISABLED;

task pidUpdate(){

	while (true){

		int count = 0;
		displayTextLine(2, "%d", count);
		for(int curController = 0; curController < NUM_CONTROLLERS; curController++){
				updateController(controllers[curController]);
		}
		count ++;
		wait10Msec(1);
	}

}

task main()
{
	initializeController(controllers[0], 0.5, 0.0000, MOTOR_ENCODER, motorA, motorA);
	setInputRange(controllers[0], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[0], -100, 100);

		initializeController(controllers[1], 0.5, 0.0001, MOTOR_ENCODER, motorC, motorC);
	setInputRange(controllers[1], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[1], -100, 100);

	state = RUNNING;
	startTask(pidUpdate);
	while(state == RUNNING){

		displayTextLine(0, "%.3f %.3f", getInput(controllers[0]), controllers[0].setpoint);
		if(onTarget(controllers[0]))
			displayTextLine(1, "On target");
		else
			displayTextLine(1, "Not on target");
		displayTextLine(3, "%.2f", getOutput(controllers[0]));

		if(nNxtButtonPressed == 1 && onTarget(controllers[0])){
			setSetpoint(controllers[0], 100);
			setSetpoint(controllers[1], 100);
		}
		else if(nNxtButtonPressed == 2 && onTarget(controllers[0])){
			setSetpoint(controllers[0], -100);
			setSetpoint(controllers[1], -100);
		}
		else if(nNxtButtonPressed == 3 && onTarget(controllers[0])){
			setSetpoint(controllers[0], 0);
			setSetpoint(controllers[1], 0);
		}

	}
}
