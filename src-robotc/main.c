#include "main.h"

const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];
RobotState state = DISABLED;

const tMotor yAxisMotor = motorA;
const tMotor xAxisMotor = motorB;
const tMotor zAxisMotor = motorC;

const tSensors yAxisLim = S1;
const tSensors xAxisLim = S2;
const tSensors colorSensor = S3;
const tSensors soundSensor = S4;

void raisePen(){
	long finalTime = time1[T1] + 1000;

	motor[zAxisMotor] = 40;
	while(time1[T1] < finalTime);

	motor[zAxisMotor] = 0;
}

void lowerPen(){
	long finalTime = time1[T1] + 1000;

	motor[zAxisMotor] = -25;
	while(time1[T1] < finalTime);

	motor[zAxisMotor] = 0;
}

void homeAxis(){

	RobotState lastState = state;

	state = HOMING;

	bool yAxisIsHome = false;
	bool xAxisIsHome = false;

	while(!yAxisIsHome && !xAxisIsHome){

		if(!yAxisIsHome)
			motor[yAxisMotor] = 10;
		else
			motor[yAxisMotor] = 0;

	if(!xAxisIsHome)
		motor[xAxisMotor] = 10;
	else
		motor[xAxisMotor] = 0;

		yAxisIsHome = (bool) SensorValue(yAxisLim);
		xAxisIsHome = (bool) SensorValue(xAxisLim);
	}

	nMotorEncoder[yAxisMotor] = 0;
	nMotorEncoder[xAxisMotor] = 0;

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
	raisePen();
	homeAxis();

	// Initialize the PI controllers
	initializeController(controllers[0], 0.5, 0.0000, MOTOR_ENCODER, motorA, motorA);
	setInputRange(controllers[0], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[0], -100, 100);

	initializeController(controllers[1], 0.5, 0.0001, MOTOR_ENCODER, motorC, motorC);
	setInputRange(controllers[1], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[1], -100, 100);


	state = RUNNING;
	startTask(pidUpdate);
	controllers[0].isEnabled = true;
	controllers[1].isEnabled = true;

	while(state == RUNNING){

		displayTextLine(0, "%.3f %.3f", getInput(controllers[0]), controllers[0].setpoint);
		if(onTarget(controllers[0]))
			displayTextLine(1, "On target");
		else
			displayTextLine(1, "Not on target");
		displayTextLine(3, "%.2f", controllers[0].outputVal);

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
