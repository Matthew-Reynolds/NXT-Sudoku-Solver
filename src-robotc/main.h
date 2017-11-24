#ifndef MAIN_H
#define MAIN_H

// Setup all the constants and stuff needed for other files in the project

#define BRICK_PRIMARY

enum RobotState{
	STATE_STARTUP,
	STATE_HOMING,
	STATE_RUNNING,
	STATE_DISABLED,
	STATE_SHUTDOWN
};

typedef short Sudoku[9][9];

const tMotor yAxisMotor = motorA;
const tMotor xAxisMotor = motorB;
const tMotor zAxisMotor = motorC;

const tSensors yAxisLim = S1;
const tSensors xAxisLim = S2;
const tSensors colorSensor = S3;
const tSensors soundSensor = S4;


// Setup the stuff for the main.c file

#include "movement.h"
#include "bluetooth.h"
#include "util/pi_controller.h"

// Declare the 2 PI controllers to be used in this project
const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];

task homeThread();
task pidUpdateThread();

#endif
