#ifndef MAIN_H
#define MAIN_H

enum RobotState{
	STATE_STARTUP,
	STATE_HOMING,
	STATE_RUNNING,
	STATE_SENDING_BT,
	STATE_RECEIVING_BT,
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

// We've get some funky business going on here.
// Since we have no linker, sources are included rather than headers.
// Each source has its own header where it and its dependencies are defined.
// This is a very irregular organization of code but is neccesary due to
// the lack of a linker in RobotC
#ifndef MATH_H
#include "util/math.c"
#endif

#ifndef PI_CONTROLLER_H
#include "util/pi_controller.c"
#endif

#ifndef MOVEMENT_H
#include "movement.c"
#endif

#ifndef BLUETOOTH_H
#include "bluetooth.c"
#endif

#endif
