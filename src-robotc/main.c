#include "main.h"

const int NUM_CONTROLLERS = 2;
PIController controllers[NUM_CONTROLLERS];
RobotState state = STATE_DISABLED;

task homeThread(){
	// Backup the current state, and set it to 'Homing'
	RobotState lastState = state;
	state = STATE_HOMING;

	homeAxis();

	// Set the robot state back to the previous state
	state = lastState;
}

// Feed all the pid controllers
task pidUpdateThread(){

	// Continue updating the pid controllers while the robot is in the running state
	while (state == STATE_RUNNING){

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
	state = STATE_STARTUP;

	raisePen();
	startTask(homeThread);
	//TODO: Establish Bluetooth connection

	// Initialize the PI controllers
	initializeController(controllers[0], 0.5, 0.0000, MOTOR_ENCODER, motorA, motorA);
	setInputRange(controllers[0], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[0], -100, 100);

	initializeController(controllers[1], 0.5, 0.0001, MOTOR_ENCODER, motorC, motorC);
	setInputRange(controllers[1], -32767, 32767, TICKS_PER_CM);
	setOutputRange(controllers[1], -100, 100);

	// Wait until the homing thread has completed
	while(state == STATE_HOMING);



	// =*=*=*=*=*=*=*=*= MAIN LOOP =*=*=*=*=*=*=*=*=
	state = STATE_RUNNING;
	startTask(pidUpdateThread);
	controllers[0].isEnabled = true;
	controllers[1].isEnabled = true;

	Sudoku puzzle;
	Sudoku solved;
	bool puzzleIsSolved = false;

	//TODO: Find the first cell. Center the color sensor in this cell
	readPuzzle(puzzle);

	// Send the puzzle over bluetooth
	BT_Status status = BT_ERROR;
	status = sendPuzzle(puzzle, -1);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayBigTextLine(2, "Send file: %s",  getErrorMessage(status));
		playSound(soundException);
		status = sendPuzzle(puzzle, -1);
		eraseDisplay();
	}


	// Recieve the puzzle from bluetooth
	status = receivePuzzle(puzzle, puzzleIsSolved, -1);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayBigTextLine(2, "Send file: %s",  getErrorMessage(status));
		playSound(soundException);
		status = receivePuzzle(puzzle, puzzleIsSolved, -1);
		eraseDisplay();
	}


	// If the puzzle was solved, print it out.
	if(puzzleIsSolved){

		//TODO: Offset the head to center the pen in the cell rather than the color sensor
		printPuzzle(puzzle, solved);
	}

	// Otherwise, make a sad noise and print out unsolveable
	else {
		displayBigTextLine(2, "Unable to solve puzzle :(");
		playSound(soundException);
	}


	// =*=*=*=*=*=*=*=*= SHUTDOWN =*=*=*=*=*=*=*=*=
	state = STATE_SHUTDOWN;
	raisePen(); // Make sure the pen is raised
	homeAxis();	// Move the head off of the paper to see the result



	// =*=*=*=*=*=*=*=*= DONE =*=*=*=*=*=*=*=*=
	state = STATE_DISABLED;
}
