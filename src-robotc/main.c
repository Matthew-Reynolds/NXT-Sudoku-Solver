#include "main.h"

RobotState state = STATE_DISABLED;

/**
 *	Home the head of the robot.
 *	Set the robot state to STATE_HOMING. Slowly drive the
 *	two axis towards their endpoints until a limit switch
 *	is hit. Then, set the robot state back to what it was
 *	before the thread was called.
 *
 *	Author: Matthew Reynolds
 */
task homeThread(){
	// Backup the current state, and set it to 'Homing'
	RobotState lastState = state;
	state = STATE_HOMING;

	homeAxis();

	// Set the robot state back to the previous state
	state = lastState;
}


/**
 *	While the robot is in operation (state is STATE_RUNNING),
 *	feed all the PID controllers in the robot every ~5ms
 *
 *	Author: Matthew Reynolds
 */
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


/**
 *	Output a verbose message for specified error to the
 *	screen, accompanied by a sound effect. Countdown the
 *	specified number of seconds before returning.
 *
 *	param BT_Status error
 *			A BT_Status that describes which error to output
 *
 *	param int timeout
 *			The number of seconds to wait (Default: 3)
 *
 *	Author: Matthew Reynolds
 */
void displayError(BT_Status error, int timeout = 3){
	displayCenteredTextLine(2, "Error:");
	displayCenteredTextLine(3, getStatusMessage(error));
	playSound(soundException);
	while(timeout > 0){
		displayCenteredTextLine(4, "Retrying in %d...", timeout);
		wait1Msec(1000);
		timeout--;
	}
}

/**
 *	Output the specified Sudoku to the screen.
 *	Note that only the first 8 lines will be
 *	displayed due to the screen size of the NXT.
 *
 *	Author: Matthew Reynolds
 */
void displayPuzzle(const Sudoku & sudoku){
	for(int i = 0; i < 9; i++){
		displayCenteredTextLine(i, "|%d%d%d|%d%d%d|%d%d%d|",
		sudoku[i][0],
		sudoku[i][1],
		sudoku[i][2],
		sudoku[i][3],
		sudoku[i][4],
		sudoku[i][5],
		sudoku[i][6],
		sudoku[i][7],
		sudoku[i][8]);
	}
}

/**
 *	Main function
 *
 *	Author: Matthew Reynolds
 */
task main()
{
	// Configure the sensors
	SensorType[xAxisLim] = sensorTouch;
	SensorType[yAxisLim] = sensorTouch;
	SensorType[soundSensor] = sensorSoundDB;
	SensorType[colorSensor] = sensorColorNxtFULL;

	// Set the motors to coast mode
	bFloatDuringInactiveMotorPWM = true;

	// =*=*=*=*=*=*=*=*= Startup =*=*=*=*=*=*=*=*=
	state = STATE_STARTUP;

	raisePen();
	startTask(homeThread);
	nVolume = 4;

	// Establish a Bluetooth connection
	displayCenteredTextLine(0, "Establishing BT");
	displayCenteredTextLine(1, "Connection...");
	setupBluetooth();
	BT_Status status = establishConnection(-1);
	displayCenteredTextLine(6, "%s", getStatusMessage(status));
	if(status != BT_SUCCESS){
		displayError(status, 0);
		wait1Msec(5000);
		stopAllTasks();
	}
	else {
		playSound(soundFastUpwardTones);

		eraseDisplay();
		displayCenteredTextLine(0, "Connected to");
		displayCenteredTextLine(1, "Solver!");
		wait1Msec(2000);
	}

	// Initialize the PI controllers
	initializeController(controllers[0], 0.8, 0.00004, MOTOR_ENCODER, yAxisMotor, yAxisMotor);
	//11cm per rev, 8:40 reduction
	setInputRange(controllers[0], -32767, 32767, 360.0/(12.2) * (40/8));
	setOutputRange(controllers[0], -100, 100);
	setTolerance(controllers[0], 5); // 5 degrees = 0.034cm

	initializeController(controllers[1], 0.7, 0.001, MOTOR_ENCODER, xAxisMotor, xAxisMotor);
	setInputRange(controllers[1], -32767, 32767, 360.0/2.6); // 2.555cm per rev
	setOutputRange(controllers[1], -100, 100);
	setTolerance(controllers[1], 5); // 5 degrees = 0.036cm

	// Wait until the homing thread has completed
	while(state == STATE_HOMING);

	// =*=*=*=*=*=*=*=*= MAIN BODY =*=*=*=*=*=*=*=*=
	state = STATE_RUNNING;
	setSetpoint(controllers[0], 0);
	setSetpoint(controllers[1], 0);
	startTask(pidUpdateThread);
	controllers[0].isEnabled = true;
	controllers[1].isEnabled = true;

	Sudoku solved;
	Sudoku puzzle;
	bool puzzleIsSolved = false;
	readPuzzle(puzzle);

	// To demonstrate that the robot can store the puzzle
	displayPuzzle(puzzle);
	wait1Msec(3000);
	eraseDisplay();

	// Send the puzzle over Bluetooth
	eraseDisplay();
	displayCenteredTextLine(0, "Sending puzzle:");
	status = sendPuzzle(puzzle, false);
	while(status != BT_SUCCESS){

		// Every time there is an error, output the error message and make a sad sound
		displayError(status);
		status = sendPuzzle(puzzle, false);
	}

	// Confirm that we have sent the puzzle
	playSound(soundFastUpwardTones);
	eraseDisplay();
	displayCenteredTextLine(0, "Sent puzzle");
	displayCenteredTextLine(1, "Waiting for soln");
	wait1Msec(1000);

	// Receive the puzzle from Bluetooth
	status = receivePuzzle(solved, puzzleIsSolved, -1);
	while(status != BT_SUCCESS){

		// Every time there is an error, output the error message and make a sad sound
		displayError(status);
		status = receivePuzzle(solved, puzzleIsSolved, -1);
	}

	// Confirm the solution has been received
	playSound(soundFastUpwardTones);
	eraseDisplay();
	displayCenteredTextLine(0, "Recieved soln");
	wait1Msec(1000);

		// To demonstrate that the robot can solve and receive the puzzle
	displayPuzzle(solved);
	wait1Msec(3000);
	eraseDisplay();

	// If the puzzle was solved, print it out.
	if(puzzleIsSolved)
		printPuzzle(puzzle, solved);

	// Otherwise, make a sad noise and print out unsolvable
	else {
		displayCenteredTextLine(0, "Unable to solve");
		displayCenteredTextLine(1, "puzzle :(");
		playSound(soundException);
	}


	// =*=*=*=*=*=*=*=*= SHUTDOWN =*=*=*=*=*=*=*=*=
	state = STATE_SHUTDOWN;
	raisePen(); // Make sure the pen is raised
	homeAxis();	// Move the head off of the paper to see the result


	// =*=*=*=*=*=*=*=*= DONE =*=*=*=*=*=*=*=*=
	state = STATE_DISABLED;
}

// This is kinda messy at the bottom but the linker should only really operate at the end
#include "linker.h"
