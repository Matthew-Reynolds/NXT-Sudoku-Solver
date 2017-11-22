#include "movement.h"

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
	for (int curLine = 8; curLine >=0; curLine--)

	// Read the current row of the puzzle
	readRow(sudoku, curLine);
}

// Read the current row from the puzzle
void readRow(Sudoku & sudoku, int curLine){

	// If the current row is odd, move to the right
	if (curLine & 1){

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 0; curColumn < 9; curColumn++){
			moveToCell(curLine, curColumn, true);

			int val = getCellValue();
			while(val < 0){
				if(curColumn != 8)
					moveToCell(curLine+1, curColumn+1, true);
				else
					moveToCell(curLine+1, curColumn-1, true);
				moveToCell(curLine, curColumn, true);
				val = getCellValue();
			}

			sudoku[curLine][curColumn] = val;
			displayCenteredTextLine(2, "Read cell:");
			displayCenteredTextLine(3, "%d", sudoku[curLine][curColumn]);
			//		wait1Msec(1000);
		}
	}

	// If the current row is even, move to the left
	else {

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 8; curColumn > -1; curColumn--){
			moveToCell(curLine, curColumn, true);

			int val = getCellValue();
			while(val < 0){
				if(curColumn != 0)
					moveToCell(curLine+1, curColumn-1, true);
				else
					moveToCell(curLine+1, curColumn+1, true);
				moveToCell(curLine, curColumn, true);
				val = getCellValue();
			}

			sudoku[curLine][curColumn] = val;
			displayCenteredTextLine(2, "Read cell:");
			displayCenteredTextLine(3, "%d", sudoku[curLine][curColumn]);
			//		wait1Msec(1000);
		}
	}
}



// Read the puzzle on the page
void printPuzzle(const Sudoku & unsolved, const Sudoku & solved){

	// For every row in the puzzle...
	for(int curLine = 0; curLine < 9; curLine++)

	// Print the current row
	printRow(unsolved, solved, curLine);
}

void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine){

	// If the current row is odd, move to the left
	if (curLine & 1){

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 8; curColumn > -1; curColumn--){
			if(unsolved[curLine][curColumn] == 0){
				moveToCell(curLine, curColumn, false);
				displayCenteredTextLine(0, "Printing '%d'", solved[curLine][curColumn]);
				displayCenteredTextLine(1, "In cell (%d,%d)", curColumn, curLine);
				plotNumber(solved[curLine][curColumn], 100);
				wait1Msec(100);
			}
		}
	}

	// If the current row is even, move to the right
	else {

		// Iterate through every column. Read the current cell.
		// If this is not the last cell, move over 1 cell
		for (int curColumn = 0; curColumn < 9; curColumn++){
			if(unsolved[curLine][curColumn] == 0){
				moveToCell(curLine, curColumn, false);
				displayCenteredTextLine(0, "Printing '%d'", solved[curLine][curColumn]);
				displayCenteredTextLine(1, "In cell (%d,%d)", curColumn, curLine);
				plotNumber(solved[curLine][curColumn], 100);
				wait1Msec(100);
			}
		}
	}
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

	motor[yAxisMotor] = 0;
	motor[xAxisMotor] = 0;

	// Once both motors have reached the origin, set the encoders to 0
	nMotorEncoder[yAxisMotor] = 0;
	nMotorEncoder[xAxisMotor] = 0;
}

// ** Zero indexed
void moveToCell(int row, int col, bool isRead){
	if(row > 8 || row < 0 ||
		col > 8 || col < 0)
	return;

	float cellHeight = 2;
	float cellWidth = 2;

	float xOffset = 120/controllers[1].scale;
	float yOffset = 775/controllers[0].scale;

	float x = (8-col)*cellWidth + xOffset;
	float y = (8-row)*cellHeight + yOffset;

	if(!isRead)
		y -= 4.5; // Offset between the read and write head

	// Set the setpoint to the new coords, and wait until we reach the cell
	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	displayCenteredTextLine(0, "Moving to");
	displayCenteredTextLine(1, "(%d, %d)", col, row);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));
}

// Move the head relative to the current position by x, y
void moveToOffset(float x, float y, long delay = 0){
	y += getScaledInput(controllers[0]);
	x += getScaledInput(controllers[1]);

	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));

	wait1Msec(delay);
}

// Write out the number specified by value.
// Assume the head starts at the center of the number.
void plotNumber(int value, long delay, float segH, float segW){

	if(value == 1){
		moveToOffset(0, segH, delay);
		lowerPen();
		moveToOffset(0, -segH*2, delay);
	}
	else if(value == 2){
		moveToOffset(segW/2, segH, delay);
		lowerPen();
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(-segW, 0, delay);
	}
	else if(value == 3){
		moveToOffset(segW/2, segH, delay);
		lowerPen();
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(segW, 0, delay);
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(segW, 0, delay);
	}
	else if(value == 4){
		moveToOffset(segW/2, segH, delay);
		lowerPen();
		moveToOffset(0, -segH, delay);
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, segH, delay);
		moveToOffset(0, -segH*2, delay);
	}
	else if(value == 5){
		moveToOffset(-segW/2, segH, delay);
		lowerPen();
		moveToOffset(segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(segW, 0, delay);
	}
	else if(value == 6){
		moveToOffset(-segW/2, segH, delay);
		lowerPen();
		moveToOffset(segW, 0, delay);
		moveToOffset(0, -segH*2, delay);
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, segH, delay);
		moveToOffset(segW, 0, delay);
	}
	else if(value == 7){
		moveToOffset(segW/2, segH, delay);
		lowerPen();
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH*2, delay);
	}
	else if(value == 8){
		moveToOffset(segW/2, -segH, delay);
		lowerPen();
		moveToOffset(0, segH*2, delay);
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, -segH*2, delay);
		moveToOffset(segW, 0, delay);
		moveToOffset(0, segH, delay);
		moveToOffset(-segW, 0, delay);
	}
	else if(value == 9){
		moveToOffset(segW/2, -segH, delay);
		lowerPen();
		moveToOffset(-segW, 0, delay);
		moveToOffset(0, segH*2, delay);
		moveToOffset(segW, 0, delay);
		moveToOffset(0, -segH, delay);
		moveToOffset(-segW, 0, delay);
	}

	raisePen();
}
