#include "movement.h"


/**
*	Determine by how much to slow down the robot based
*	on the reading from the sound sensor
*
*	return int
*			The delay value, from 0-500
*/
int getDelay(){
	long sound = SensorValue[soundSensor]; // 0-100
	return 500 - (sound*5); // 500-0
}


/**
*	Raise the pen off the paper.
*	Note this function blocks until the procedure is complete.
*/
void raisePen(){
	long finalTime = time1[T1] + 300;

	motor[zAxisMotor] = 100;
	while(time1[T1] < finalTime);
}


/**
*	Lower the pen to the paper.
*	Note this function blocks until the procedure is complete.
*/
void lowerPen(){
	long finalTime = time1[T1] + 400;

	motor[zAxisMotor] = -10;
	while(time1[T1] < finalTime);
	motor[zAxisMotor] = 0;
}


/**
*	Read the puzzle from the paper into the specified
*	sudoku object.
*
*	param Sudoku & sudoku
*			The puzzle to populate with data from the page
*/
void readPuzzle(Sudoku & sudoku){

	// Iterate through each line of the puzzle
	// Read the current row
	for (int curLine = 8; curLine >=0; curLine--)
		readRow(sudoku, curLine);
}


/**
*	Read the specified row of the puzzle from the paper
*	into the specified sudoku object.
*
*	param Sudoku & sudoku
*			The puzzle to populate with data from the page
*
*	param int curLine
*			The line of the puzzle to read
*/
void readRow(Sudoku & sudoku, int curLine){

	// If the current row is odd, move to the right
	if (curLine & 1){

		// Iterate through every column. Read the current cell.
		for (int curColumn = 0; curColumn < 9; curColumn++){
			moveToCell(curLine, curColumn, true);
			int val = getCellValue();

			// If the color sensor was unable to determine the color,
			// move the head off of and back to the current cell to realign it
			while(val < 0){
				int resetRow = (curLine == 8) ? curLine - 1 : curLine + 1;
				int resetCol = (curColumn == 8) ? curColumn + 1 : curColumn - 1;
				moveToCell(resetRow,resetCol, true);
				moveToCell(curLine, curColumn, true);
				moveToCell(curLine, curColumn, true);
				val = getCellValue();
			}

			// Save the read value into the sudoku object and print out the result
			sudoku[curLine][curColumn] = val;
			displayCenteredTextLine(0, "Read cell:");
			displayCenteredTextLine(1, "%d", sudoku[curLine][curColumn]);
			wait1Msec(getDelay());
		}
	}

	// If the current row is even, move to the left
	else {

		// Iterate through every column. Read the current cell.
		for (int curColumn = 8; curColumn > -1; curColumn--){
			moveToCell(curLine, curColumn, true);
			int val = getCellValue();

			// If the color sensor was unable to determine the color,
			// move the head off of and back to the current cell to realign it
			while(val < 0){
				int resetRow = (curLine == 8) ? curLine - 1 : curLine + 1;
				int resetCol = (curColumn == 0) ? curColumn + 1 : curColumn - 1;
				moveToCell(resetRow,resetCol, true);
				moveToCell(curLine, curColumn, true);
				val = getCellValue();
			}

			// Save the read value into the sudoku object and print out the result
			sudoku[curLine][curColumn] = val;
			displayCenteredTextLine(0, "Read cell:");
			displayCenteredTextLine(1, "%d", sudoku[curLine][curColumn]);
			wait1Msec(getDelay());
		}
	}
}


/**
*	Print the specified puzzle onto the paper.
*
*	param const Sudoku & unsolved
*			The unsolved puzzle, used to determine the empty cells
*
*	param const Sudoku & solved
*			The solved puzzle to print out
*/
void printPuzzle(const Sudoku & unsolved, const Sudoku & solved){

	// Iterate through every row of the puzzle.
	// Print out the current row
	for(int curLine = 0; curLine < 9; curLine++)
		printRow(unsolved, solved, curLine);
}


/**
*	Print the specified row of the puzzle onto the paper.
*
*	param const Sudoku & unsolved
*			The unsolved puzzle, used to determine the empty cells
*
*	param const Sudoku & solved
*			The solved puzzle to print out
*
*	param int curLine
*			The line of the puzzle to print
*/
void printRow(const Sudoku & unsolved, const Sudoku & solved, int curLine){


	// Iterate through every column.
	for (int curColumn = 0; curColumn < 9; curColumn++){
		int workingCol = curColumn;

		// If the current row is odd, move to the left
		if (curLine & 1)
			workingCol = 8-workingCol;

		// If the cell is empty, move to this cell and print out the solution
		if(unsolved[curLine][workingCol] == 0){
			moveToCell(curLine, workingCol, false);
			displayCenteredTextLine(0, "Printing '%d'", solved[curLine][workingCol]);
			displayCenteredTextLine(1, "In cell (%d,%d)", workingCol, curLine);
			plotNumber(solved[curLine][workingCol]);
			wait1Msec(getDelay());
		}
	}
}


/**
*	Home the head of the robot.
*
*	Slowly drive the two axis towards their endpoints until
*	a limit switch is hit. Then set the encoders to 0
*/
void homeAxis(){

	// Loop until both axis have 'homed'
	bool yAxisIsHome = false;
	bool xAxisIsHome = false;
	while(!yAxisIsHome || !xAxisIsHome){

		// If the y axis has reached its origin, stop.
		// Otherwise, run at -20%
		if(SensorValue[yAxisLim]){
			motor[yAxisMotor] = 0;
			yAxisIsHome = true;
		}
		else
			motor[yAxisMotor] = -20;

		// If the x axis has reached its origin, stop.
		// Otherwise, run at -20%
		if(SensorValue[xAxisLim]){
			motor[xAxisMotor] = 0;
			xAxisIsHome = true;
		}
		else
			motor[xAxisMotor] = -20;
	}

	// Stop the motors
	motor[yAxisMotor] = 0;
	motor[xAxisMotor] = 0;

	// Once both motors have reached the origin, set the encoders to 0
	nMotorEncoder[yAxisMotor] = 0;
	nMotorEncoder[xAxisMotor] = 0;
}


/**
*	Move the head of the robot to the specified cell. The isRead
*	flag specifies whether to align the read or write head with
*	the center of the cell.
*
*	param int row
*			The row to move to
*
*	param int col
*			The column to move to
*
*	param bool isRead
*			Whether to center the read head or not
*/
void moveToCell(int row, int col, bool isRead){

	// Ensure the coordinates are valid
	if(row > 8 || row < 0 || col > 8 || col < 0)
		return;

	// Size of the cells (cm)
	float cellHeight = 2;
	float cellWidth = 2;

	// Offset from the 'home' to cell (8,8) (cm)
	float xOffset = 1.25;
	float yOffset = 5.75;

	// Calculate the position to move to, in cm
	float x = (8-col)*cellWidth + xOffset;
	float y = (8-row)*cellHeight + yOffset;

	// Offset between the read and write head as required
	if(!isRead){
		y -= 5.25;
		x -= 0.5;
	}

	// Set the setpoint to the new coords, and wait until we reach the cell
	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	displayCenteredTextLine(2, "Moving to");
	displayCenteredTextLine(3, "(%d, %d)", col, row);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));
}


/**
*	Move the head of the robot relative to its current position
*
*	param float x
*			The distance to move in the X axis (cm)
*
*	param int y
*			The distance to move in the Y axis (cm)
*
*	param long delat
*			How long to delay after the movement (ms)
*			(Default: 100)
*/
void moveToOffset(float x, float y, long delay){
	y += getScaledInput(controllers[0]);
	x += getScaledInput(controllers[1]);

	setSetpoint(controllers[0], y);
	setSetpoint(controllers[1], x);
	while(!(onTarget(controllers[0]) && onTarget(controllers[1])));
	wait1Msec(delay);
}


/**
*	Write out the specified number, centered on the
*	head's current location.
*
*	param int val
*			The value to print (0-9)
*
*	param float segH
*			The height of each segment of the number, in cm
*			(Default: 0.75)
*
*	param float segW
*			The width of each segment of the number, in cm
*			(Default: 1.0)
*/
void plotNumber(int value, float segH, float segW){

	if(value == 1){
		moveToOffset(0, segH, getDelay());
		lowerPen();
		moveToOffset(0, -segH*2, getDelay());
	}
	else if(value == 2){
		moveToOffset(segW/2, segH, getDelay());
		lowerPen();
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -segH, getDelay());
		moveToOffset(segW, 0, getDelay());
		moveToOffset(0, -segH, getDelay());
		moveToOffset(-segW, 0, getDelay());
	}
	else if(value == 3){
		moveToOffset(segW/2, 0, getDelay());
		lowerPen();
		moveToOffset(-segW, 0, getDelay());
		raisePen();
		moveToOffset(segW, segH, getDelay());
		lowerPen();
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -2*segH, getDelay());
		moveToOffset(segW, 0, getDelay());
	}
	else if(value == 4){
		/*moveToOffset(segW/2, segH, getDelay());
		lowerPen();
		moveToOffset(0, -segH, getDelay());
		moveToOffset(-segW, 0, getDelay());
		raisePen();
		moveToOffset(0, segH, getDelay());
		lowerPen();
		moveToOffset(0, -segH*2, getDelay());*/

		moveToOffset(-segW/2, 0, getDelay());
		lowerPen();
		moveToOffset(segW, 0, getDelay());
		moveToOffset(0, segH, getDelay());
		raisePen();
		moveToOffset(-segW, 0, getDelay());
		lowerPen();
		moveToOffset(0, -segH*2, getDelay());
	}
	else if(value == 5){
		moveToOffset(-segW/2, segH, getDelay());
		lowerPen();
		moveToOffset(segW, 0, getDelay());
		moveToOffset(0, -segH, getDelay());
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -segH, getDelay());
		moveToOffset(segW, 0, getDelay());
	}
	else if(value == 6){
		moveToOffset(segW/2, 0, getDelay());
		lowerPen();
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -segH, getDelay());
		moveToOffset(segW, 0, getDelay());
		moveToOffset(0, 2*segH, getDelay());
		moveToOffset(-segW, 0, getDelay());
	}
	else if(value == 7){
		moveToOffset(segW/2, segH, getDelay());
		lowerPen();
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -segH*2, getDelay());
	}
	else if(value == 8){
		moveToOffset(-segW/2, 0, getDelay());
		lowerPen();
		moveToOffset(segW, 0, getDelay());
		raisePen();
		moveToOffset(0, -segH, getDelay());
		lowerPen();
		moveToOffset(0, segH*2, getDelay());
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -segH*2, getDelay());
		moveToOffset(segW, 0, getDelay());
	}
	else if(value == 9){
		moveToOffset(-segW/2, 0, getDelay());
		lowerPen();
		moveToOffset(segW, 0, getDelay());
		moveToOffset(0, segH, getDelay());
		moveToOffset(-segW, 0, getDelay());
		moveToOffset(0, -2*segH, getDelay());
		moveToOffset(segW, 0, getDelay());
	}
	raisePen();
}
