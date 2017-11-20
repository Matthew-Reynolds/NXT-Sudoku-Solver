#include "main.h"

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
	nVolume = 4;

	// Establish a Bluetooth connection
	/*
	displayCenteredTextLine(2, "Establishing BT");
	displayCenteredTextLine(4, "Connection...");
	setupBluetooth();
	BT_Status status = establishConnection(-1);
	displayCenteredTextLine(6, "%s", getStatusMessage(status));
	if(status != BT_SUCCESS){
	playSound(soundException);
	wait1Msec(5000);
	return;
	}
	else {
	playSound(soundFastUpwardTones);
	wait1Msec(5000);
	}*/

	// Initialize the PI controllers
	initializeController(controllers[0], 0.8, 0.00005, MOTOR_ENCODER, motorA, motorA);
	setInputRange(controllers[0], -32767, 32767, 360.0/(11) * (40/8));	//11cm per rotation, 8:40 reduction
	setOutputRange(controllers[0], -100, 100);
	setTolerance(controllers[0], 2);

	initializeController(controllers[1], 0.9, 0.0001, MOTOR_ENCODER, motorB, motorB);
	setInputRange(controllers[1], -32767, 32767, 360.0/2.555); // 2.555cm per rev
	setOutputRange(controllers[1], -100, 100);
	setTolerance(controllers[1], 1);

	// Set the motors to coast mode
	bFloatDuringInactiveMotorPWM = true;

	// Wait until the homing thread has completed
	while(state == STATE_HOMING);



	// =*=*=*=*=*=*=*=*= MAIN LOOP =*=*=*=*=*=*=*=*=
	state = STATE_RUNNING;
	startTask(pidUpdateThread);
	controllers[0].isEnabled = true;
	controllers[1].isEnabled = true;


	Sudoku solved;
	Sudoku puzzle;
	bool puzzleIsSolved = false;

	// Print 1-9, then 9-1
	for(int col = 0; col < 9; col++){
		moveToCell(0,col,true);
		plotNumber(col+1);
		wait1Msec(500);
	}
	for(int col = 0; col < 9; col++){
		moveToCell(1,col,true);
		plotNumber(8-col+1);
		wait1Msec(500);
	}

	return;

	findFirstCell();
	readPuzzle(puzzle);

	// Send the puzzle over bluetooth
	BT_Status status = sendPuzzle(puzzle, false);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayBigTextLine(2, "Send file: %s",  getStatusMessage(status));
		playSound(soundException);
		status = sendPuzzle(puzzle, false);
		eraseDisplay();
	}


	// Recieve the puzzle from bluetooth
	status = receivePuzzle(puzzle, puzzleIsSolved, -1);
	while(status != BT_SUCCESS){

		// Everytime there is an error, output the error message and make a sad sound
		displayBigTextLine(2, "Send file: %s",  getStatusMessage(status));
		playSound(soundException);
		status = receivePuzzle(solved, puzzleIsSolved, -1);
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
