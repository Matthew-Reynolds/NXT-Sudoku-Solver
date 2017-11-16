#include "movement.h"

// Raise the pen off the paper
void raisePen(){
	long finalTime = time1[T1] + 1000;

	motor[zAxisMotor] = 40;
	while(time1[T1] < finalTime);

	motor[zAxisMotor] = 0;
}

// Lower the pen down to the paper
void lowerPen(){
	long finalTime = time1[T1] + 1000;

	motor[zAxisMotor] = -25;
	while(time1[T1] < finalTime);

	motor[zAxisMotor] = 0;
}


// Read the puzzle from the page
void readPuzzle(Sudoku & sudoku){

	// Iterate through each line of the puzzle
	for (int curLine = 0; curLine < 9; curLine++){

		// Read the current row of the puzzle
		readRow(sudoku, curLine);

		// If there are more rows...
		if (curLine != 8){
			if(curLine & 1)
				moveToCell(curLine+1, 0);
			else
				moveToCell(curLine+1, 8);
		}
	}
}

// Read the current row from the puzzle
void readRow(Sudoku & sudoku, int curLine){

	// If the current row is odd, move to the left
	if (curLine & 1)
		for (int curColumn = 8; curColumn > -1; curColumn--) {
		sudoku[curLine][curColumn] = getCellValue();

		if (curColumn != 0)
			moveToCell(curLine, curColumn - 1);
	}

	// If the current row is even, move to the right
	else
		for (int curColumn = 0; curColumn < 9; curColumn++) {
		sudoku[curLine][curColumn] = getCellValue();

		if (curColumn != 8)
			moveToCell(curLine,curColumn + 1);
	}
}


void printPuzzle(const Sudoku & unsolved, const Sudoku & solved){
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

void findFirstCell(){
	/*	SensorType[colorSensor] = sensorColorNxtFULL;
	long waitTime = time[T1] + 50;
	while(time[T1] < waitTime);

	motor[xAxis] = -10;
	while(SensorValue[colorSensor] == );
	motor[xAxis] = 0;

	motor[yAxis] = -10;
	while(SensorValue[colorSensor] != );
	motor[yAxis] = 0;*/
}

void moveToCell(int row, int col){
}
