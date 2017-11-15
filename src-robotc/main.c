#include "main.h"

const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];
RobotState state = DISABLED;

task home(){
	// Backup the current state, and set it to 'Homing'
	RobotState lastState = state;
	state = HOMING;

	homeAxis();

	// Set the robot state back to the previous state
	state = lastState;
}

// Feed all the pid controllers
task pidUpdate(){

	// Continue updating the pid controllers until the robot enters its shutdown routine
	while (state != SHUTDOWN){

		// Iterate through every controller, and feed it
		for(int curController = 0; curController < NUM_CONTROLLERS; curController++)
			if(controllers[curController].isEnabled)
			updateController(controllers[curController]);

		// Delay for 5ms so that we don't pin the CPU
		wait1Msec(5);
	}
}

task main()
{
	// Configure the sensors
	SensorType[xAxisLim] = sensorTouch;
	SensorType[yAxisLim] = sensorTouch;
	SensorType[soundSensor] = sensorSoundDB;
	SensorType[colorSensor] = sensorColorNxtFULL;



	// =*=*=*=*=*=*=*=*= Startup =*=*=*=*=*=*=*=*=
	state = STARTUP;

	raisePen();
	startTask(home);
	//TODO: Establish Bluetooth connection

	// Initialize the PI controllers
	initializeController(controllers[0], 0.5, 0.0000, MOTOR_ENCODER, motorA, motorA);
	setInputRange(controllers[0], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[0], -100, 100);

	initializeController(controllers[1], 0.5, 0.0001, MOTOR_ENCODER, motorC, motorC);
	setInputRange(controllers[1], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[1], -100, 100);

	// Wait until the homing thread has completed
	while(state == HOMING);



	// =*=*=*=*=*=*=*=*= MAIN LOOP =*=*=*=*=*=*=*=*=
	state = RUNNING;
	startTask(pidUpdate);
	controllers[0].isEnabled = true;
	controllers[1].isEnabled = true;

	while(state == RUNNING){




	}
}
