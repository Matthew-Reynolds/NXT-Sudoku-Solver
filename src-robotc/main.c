#include "main.h"

Sudoku board1 =
{2, 0, 0, 0, 4, 0, 0, 3, 8,
	0, 3, 0, 0, 0, 2, 0, 1, 9,
	0, 9, 8, 3, 0, 1, 6, 2, 0,
	0, 0, 3, 2, 0, 0, 1, 0, 0,
	0, 5, 0, 0, 0, 0, 0, 6, 0,
	0, 0, 6, 0, 0, 7, 2, 0, 0,
	0, 7, 5, 4, 0, 3, 9, 8, 0,
	1, 2, 0, 7, 0, 0, 0, 5, 0,
	3, 8, 0, 0, 6, 0, 0, 0, 2};

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

void displayError(BT_Status error, int timeout = 3){
	displayCenteredTextLine(2, "Error:");
	displayCenteredTextLine(3, getStatusMessage(error));
	playSound(soundException);
	for(;timeout > 0; timeout--){
		displayCenteredTextLine(4, "Retrying in %d...", timeout);
		wait1Msec(1000);
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
		return;
	}
	else {
		playSound(soundFastUpwardTones);

		eraseDisplay();
		displayCenteredTextLine(0, "Connected to");
		displayCenteredTextLine(1, "Solver!");
		wait1Msec(2000);
	}

	// Initialize the PI controllers
	initializeController(controllers[0], 0.8, 0.00005, MOTOR_ENCODER, yAxisMotor, yAxisMotor);
	setInputRange(controllers[0], -32767, 32767, 360.0/(12.2) * (40/8));	//11cm per rotation, 8:40 reduction
	setOutputRange(controllers[0], -100, 100);
	setTolerance(controllers[0], 3); // 3 degrees = 0.02cm

//	initializeController(controllers[1], 0.9, 0.0005, MOTOR_ENCODER, motorB, motorB);
	initializeController(controllers[1], 0.7, 0.001, MOTOR_ENCODER, xAxisMotor, xAxisMotor);
	setInputRange(controllers[1], -32767, 32767, 360.0/2.6); // 2.555cm per rev
	setOutputRange(controllers[1], -100, 100);
	setTolerance(controllers[1], 3); // 3 degrees = 0.02cm
	//Old: 1

	// Set the motors to coast mode
	bFloatDuringInactiveMotorPWM = true;

	// Wait until the homing thread has completed
	while(state == STATE_HOMING);



	// =*=*=*=*=*=*=*=*= MAIN LOOP =*=*=*=*=*=*=*=*=
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
	//printPuzzle(puzzle);
	//wait1Msec(5000);

	memcpy(puzzle, board1, sizeof(board1[0][0])*81);

	eraseDisplay();
	displayCenteredTextLine(0, "Sending puzzle:");
	// Send the puzzle over bluetooth
	status = sendPuzzle(puzzle, false);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayError(status);
		status = sendPuzzle(puzzle, false);
	}

	playSound(soundFastUpwardTones);
	eraseDisplay();
	displayCenteredTextLine(0, "Sent puzzle");
	displayCenteredTextLine(1, "Waiting for soln");
	wait1Msec(1000);

	// Recieve the puzzle from bluetooth
	status = receivePuzzle(solved, puzzleIsSolved, -1);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayError(status);
		status = receivePuzzle(solved, puzzleIsSolved, -1);
	}

	playSound(soundFastUpwardTones);
	eraseDisplay();
	displayCenteredTextLine(0, "Recieved soln");
	wait1Msec(1000);

	// If the puzzle was solved, print it out.
	if(puzzleIsSolved)
		printPuzzle(puzzle, solved);

	// Otherwise, make a sad noise and print out unsolveable
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
