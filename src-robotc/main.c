#include "main.h"

const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];
RobotState state = DISABLED;

// Populate the board with the following pattern for fun:
// -------------------
// |1 2 3|4 5 6|7 8 9|
// |2 3 4|5 6 7|8 9 1|
// |3 4 5|6 7 8|9 1 2|
// -------------------
// |4 5 6|7 8 9|1 2 3|
// |5 6 7|8 9 1|2 3 4|
// |6 7 8|9 1 2|3 4 5|
// -------------------
// |7 8 9|1 2 3|4 5 6|
// |8 9 1|2 3 4|5 6 7|
// |9 1 2|3 4 5|6 7 8|
// -------------------
void solvePuzzle(Sudoku & sudoku){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			int val = ((j+1) + i);
			if(val > 9)
				val %= 9;
			sudoku.board[i][j] = val;
		}
	}
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

	// Wrapping the 2D array in a structure called "Sudoku" allows us to pass the val by ref
	Sudoku sudoku;
	solvePuzzle(sudoku);

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
