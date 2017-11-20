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
				moveToCell(curLine+1, 0, true);
			else
				moveToCell(curLine+1, 8, true);
		}
	}
}

// Read the current row from the puzzle
void readRow(Sudoku & sudoku, int curLine){

	// If the current row is odd, move to the left
	if (curLine & 1){

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 8; curColumn > -1; curColumn--){
			sudoku[curLine][curColumn] = getCellValue();
			displayCenteredTextLine(0, "Read cell:");
			displayCenteredTextLine(1, "%d", sudoku[curLine][curColumn];
			wait1Msec(2000);
			if (curColumn != 0)
				moveToCell(curLine, curColumn - 1, true);
			wait1Msec(2000);
		}
	}

	// If the current row is even, move to the right
	else{

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 0; curColumn < 9; curColumn++){
			sudoku[curLine][curColumn] = getCellValue();
			displayCenteredTextLine(0, "Read cell:");
			displayCenteredTextLine(1, "%d", sudoku[curLine][curColumn];
			wait1Msec(2000);
			if (curColumn != 8)
				moveToCell(curLine,curColumn + 1, true);

			wait1Msec(2000);
		}
	}
}


void printPuzzle(const Sudoku & unsolved, const Sudoku & solved){
}

void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine){
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
			motor[yAxisMotor] = -20;

		// If the x axis has reached its origin, stop.
		// Otherwise, run at -10%
		if(SensorValue[xAxisLim]){
			motor[xAxisMotor] = 0;
			xAxisIsHome = true;
		}
		else
			motor[xAxisMotor] = -20;
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

// ** Zero indexed
void moveToCell(int row, int col, bool isRead){
	float cellHeight = 2;
	float cellWidth = 2;

	float xOffset = 0;
	float yOffset = 0;

	float x = col*cellWidth + xOffset;
	float y = row*cellHeight + yOffset;

	if(isRead)
		y += 0; // Offset

	// Set the setpoint to the new coords, and wait until we reach the cell
	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	displayCenteredTextLine(0, "Moving to");
	displayCenteredTextLine(1, "(%d, %d)", col, row);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));
}

// Move the head relative to the current position by x, y
void moveToOffset(float x, float y){
	y += getScaledInput(controllers[0]);
	x += getScaledInput(controllers[1]);

	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));
}

// Write out the number specified by value.
// Assume the head starts at the center of the number.
void plotNumber(int value, float segH, float segW, long delay){

	if(value == 1){
		moveToOffset(0, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(0, segH*2);
		wait1Msec(delay);
	}
	else if(value == 2){
		moveToOffset(-segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
	}
	else if(value == 3){
		moveToOffset(-segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
	}
	else if(value == 4){
		moveToOffset(-segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, -segH);
		wait1Msec(delay);
		moveToOffset(0, segH*2);
		wait1Msec(delay);
	}
	else if(value == 5){
		moveToOffset(segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(-segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
	}
	else if(value == 6){
		moveToOffset(segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(-segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH*2);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, -segH);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
	}
	else if(value == 7){
		moveToOffset(-segW/2, -segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH*2);
		wait1Msec(delay);
	}
		else if(value == 8){
		moveToOffset(-segW/2, 0);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(0, -segH);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH*2);
		wait1Msec(delay);
			moveToOffset(-segW, 0);
		wait1Msec(delay);
			moveToOffset(0, -segH);
		wait1Msec(delay);
					moveToOffset(segW, 0);
		wait1Msec(delay);
	}
	else if(value == 9){
		moveToOffset(-segW/2, segH);
		wait1Msec(delay);
		lowerPen();
		moveToOffset(segW, 0);
		wait1Msec(delay);
		moveToOffset(0, -segH*2);
		wait1Msec(delay);
		moveToOffset(-segW, 0);
		wait1Msec(delay);
		moveToOffset(0, segH);
		wait1Msec(delay);
		moveToOffset(segW, 0);
		wait1Msec(delay);
	}

	raisePen();
}
