#include "main.h"

task main()
{
	PIController x_axis_controller;
	initializeController(x_axis_controller, 0.1, 0.2, getMotorEncoder(motorA));
}
