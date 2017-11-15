#include "movement.h"


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

	// Loop until both axis have 'homed'
	bool yAxisIsHome = false;
	bool xAxisIsHome = false;
	while(!yAxisIsHome || !xAxisIsHome){

		// If the y axis has reached its origin, stop.
		// Otherwise, run at -10%
		if(SensorValue[yAxisLim]){
			motor[yAxisMotor] = 0;
			yAxisIsHome = true;
		}
		else
			motor[yAxisMotor] = -10;

		// If the x axis has reached its origin, stop.
		// Otherwise, run at -10%
		if(SensorValue[xAxisLim]){
			motor[xAxisMotor] = 0;
			xAxisIsHome = true;
		}
		else
			motor[xAxisMotor] = -10;
	}

	// Once both motors have reached the origin, set the encoders to 0
	nMotorEncoder[yAxisMotor] = 0;
	nMotorEncoder[xAxisMotor] = 0;
}
