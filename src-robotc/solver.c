#include "solver.h"

// Easy
short board1[9][9] =
{2, 0, 0, 0, 4, 0, 0, 3, 8,
	0, 3, 0, 0, 0, 2, 0, 1, 9,
	0, 9, 8, 3, 0, 1, 6, 2, 0,
	0, 0, 3, 2, 0, 0, 1, 0, 0,
	0, 5, 0, 0, 0, 0, 0, 6, 0,
	0, 0, 6, 0, 0, 7, 2, 0, 0,
	0, 7, 5, 4, 0, 3, 9, 8, 0,
	1, 2, 0, 7, 0, 0, 0, 5, 0,
	3, 8, 0, 0, 6, 0, 0, 0, 2};


// Medium
short board2[9][9] =
{6, 0, 0, 0, 0, 7, 0, 0, 5,
	0, 1, 0, 5, 0, 6, 0, 0, 0,
	0, 0, 9, 0, 0, 0, 7, 1, 0,
	0, 8, 0, 0, 0, 0, 3, 4, 2,
	0, 0, 0, 6, 0, 3, 0, 0, 0,
	1, 2, 3, 0, 0, 0, 0, 6, 0,
	0, 7, 1, 0, 0, 0, 6, 0, 0,
	0, 0, 0, 3, 0, 5, 0, 2, 0,
	4, 0, 0, 1, 0, 0, 0, 0, 7};


// Hard
short board3[9][9] =
{8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 3, 6, 0, 0, 0, 0, 0,
	0, 7, 0, 0, 9, 0, 2, 0, 0,
	0, 5, 0, 0, 0, 7, 0, 0, 0,
	0, 0, 0, 0, 4, 5, 7, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 3, 0,
	0, 0, 1, 0, 0, 0, 0, 6, 8,
	0, 0, 8, 5, 0, 0, 0, 1, 0,
	0, 9, 0, 0, 0, 0, 4, 0, 0};



short getBoxNumber(short row, short col){
	return (short)(col/3) + 3*(row/3);
}

short getBoxEntryCol(short box, short entry){
	return 3*(box%3) + (entry%3);
}

short getBoxEntryRow(short box, short entry){
	return 3*(box/3) + (entry/3);
}

bool checkValid(const Sudoku & sudoku){
	bool isValid = true;

	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0,0,0,0,0,0,0,0,0};
		short colInstances[9] = {0,0,0,0,0,0,0,0,0};
		short boxInstances[9] = {0,0,0,0,0,0,0,0,0};

		for(int minor = 0; minor < 9; minor++){
			short val = 0;

			if(val = sudoku.board[major][minor])
				rowInstances[val-1]++;
			if(val = sudoku.board[minor][major])
				colInstances[val-1]++;
			if(val = sudoku.board[getBoxEntryRow(major, minor)]
				[getBoxEntryCol(major, minor)])
			boxInstances[val-1]++;
		}

		// Everything should not appear, or appear exactly once
		for(int check = 0; check < 9 && isValid; check++){
			isValid &= (rowInstances[check] < 2);
			isValid &= (colInstances[check] < 2);
			isValid &= (boxInstances[check] < 2);
		}
	}

	return isValid;
}

bool checkSolved(const Sudoku & sudoku){
	bool isValid = true;

	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0,0,0,0,0,0,0,0,0};
		short colInstances[9] = {0,0,0,0,0,0,0,0,0};
		short boxInstances[9] = {0,0,0,0,0,0,0,0,0};

		for(int minor = 0; minor < 9; minor++){
			short val = 0;

			if(val = sudoku.board[major][minor])
				rowInstances[val-1]++;
			if(val = sudoku.board[minor][major])
				colInstances[val-1]++;
			if(val = sudoku.board[getBoxEntryRow(major, minor)]
				[getBoxEntryCol(major, minor)])
			boxInstances[val-1]++;
		}

		// Everything should appear exactly once
		for(int check = 0; check < 9 && isValid; check++){
			isValid &= (rowInstances[check] == 1);
			isValid &= (colInstances[check] == 1);
			isValid &= (boxInstances[check] == 1);
		}
	}
	return isValid;
}

bool checkCol(short * potentialColVals,
short * potentialRowVals,
short * potentialBoxVals,
Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each col, one at a time
	for(short col = 0; col < 9; col++){
		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleRow[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every number 1-9, and get how many spots this number can be placed in this column
		for(short row = 0; row < 9; row++){
			if(sudoku.board[row][col] < 1){
				short newBit = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];


				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleRow[bit] = row;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku.board[possibleRow[curVal]][col] = curVal+1;

				potentialColVals[col] -= decToBit(curVal+1);
				potentialRowVals[possibleRow[curVal]] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(possibleRow[curVal], col)] -= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}

bool checkRow(short * potentialColVals,
short * potentialRowVals,
short * potentialBoxVals,
Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each row, one at a time
	for(short row = 0; row < 9; row++){

		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleCol[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every number 1-9, and get how many spots this number can be placed in this row
		for(short col = 0; col < 9; col++){
			if(sudoku.board[row][col] < 1){
				short newBit = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];

				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleCol[bit] = col;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku.board[row][possibleCol[curVal]] = curVal+1;

				potentialColVals[possibleCol[curVal]] -= decToBit(curVal+1);
				potentialRowVals[row] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(row, possibleCol[curVal])] -= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}

bool checkBox(short * potentialColVals,
short * potentialRowVals,
short * potentialBoxVals,
Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each box, one at a time
	for(short box = 0; box < 9; box++){
		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleEntry[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every entry 1-9, and get how many spots this number can be placed in this row
		for(short entry = 0; entry < 9; entry++){
			if(sudoku.board[getBoxEntryRow(box, entry)][getBoxEntryCol(box, entry)] < 1){
				short newBit = potentialColVals[getBoxEntryCol(box, entry)] &
				potentialRowVals[getBoxEntryRow(box, entry)] &
				potentialBoxVals[box];

				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleEntry[bit] = entry;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku.board[getBoxEntryRow(box, possibleEntry[curVal])]
				[getBoxEntryCol(box, possibleEntry[curVal])]
				= curVal+1;

				potentialColVals[getBoxEntryCol(box, possibleEntry[curVal])] -= decToBit(curVal+1);
				potentialRowVals[getBoxEntryRow(box, possibleEntry[curVal])] -= decToBit(curVal+1);
				potentialBoxVals[box] -= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}


bool reynoldsSolverInner(short * potentialColVals,
short * potentialRowVals,
short * potentialBoxVals,
Sudoku & sudoku){

	// Make a copy of the current board state so that we can revert if this recursion fails
	short saveBoard[9][9];
	short saveColVals[9];
	short saveRowVals[9];
	short saveBoxVals[9];
	copyArray(sudoku.board, saveBoard, 81);
	copyArray(potentialColVals, saveColVals, 9);
	copyArray(potentialRowVals, saveRowVals, 9);
	copyArray(potentialBoxVals, saveBoxVals, 9);

	// Make as many changes using logic as possible
	bool madeChange = false;
	do{
		madeChange = checkCol(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);
		madeChange |= checkRow(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);
		madeChange |= checkBox(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);

	} while(madeChange);


	// Check if we've solved it
	bool isSolved = checkSolved(sudoku);

	// If logic can't get us any further and the puzzle still isn't solved, we
	// need to try a little bit of trial-&-error / backtracking.

	// Find the first empty cell
	bool gotCell = false;
	for(short row = 0; row < 9 && !isSolved && !gotCell; row++){
		for(short col = 0; col < 9 && !isSolved && !gotCell; col++){
			if(sudoku.board[row][col] == 0){
				gotCell = true;

				// Get the potential vals that can go here
				short potVals = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];

				// For each potential val...
				for(short curVal = 1; curVal <= 9 && !isSolved; curVal++){
					if(decToBit(curVal) & potVals){

						// Sub in the val and RECURSION
						sudoku.board[row][col] = curVal;
						potentialRowVals[row] -= decToBit(curVal);
						potentialColVals[col] -= decToBit(curVal);
						potentialBoxVals[getBoxNumber(row, col)] -= decToBit(curVal);

						isSolved = reynoldsSolverInner(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);

						// If it's solved, great! Exit. If not, reset this val and try the next one.
						if(!isSolved){
							sudoku.board[row][col] = 0;
							potentialColVals[col] += decToBit(curVal);
							potentialRowVals[row] += decToBit(curVal);
							potentialBoxVals[getBoxNumber(row, col)] += decToBit(curVal);
						}
					}
				}
			}
		}
	}

	if(!isSolved){
		copyArray(saveBoard, sudoku.board, 81);
		copyArray(saveColVals, potentialColVals, 9);
		copyArray(saveRowVals, potentialRowVals, 9);
		copyArray(saveBoxVals, potentialBoxVals, 9);

	}

	return isSolved;
}

bool reynoldsSolver(Sudoku & sudoku){
	if(!checkValid(sudoku))
		return false;


	short potentialColVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};
	short potentialRowVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};
	short potentialBoxVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};

	// Determine which vals are not used in each row, col, and box
	for(short row = 0; row < 9; row++){
		for(short col = 0; col < 9; col++){
			short cellBit = decToBit(sudoku.board[row][col]);
			potentialColVals[col] -= cellBit;
			potentialRowVals[row] -= cellBit;
			potentialBoxVals[getBoxNumber(row, col)] -= cellBit;
		}
	}
	return reynoldsSolverInner(potentialColVals,	potentialRowVals, potentialBoxVals, sudoku);
}

task main()
{
	Sudoku sudoku;

	while(true){

		// Wait until a button is pressed
		while(nNxtButtonPressed == -1);

		if(nNxtButtonPressed == 2){
			eraseDisplay();
			displayTextLine(0, "Solving Easy");
			copyArray(board1, sudoku.board, 81);
		}
		else if(nNxtButtonPressed == 3){
			eraseDisplay();
			displayTextLine(0, "Solving Med");
			copyArray(board2, sudoku.board, 81);
		}
		else if(nNxtButtonPressed == 1){
			eraseDisplay();
			displayTextLine(0, "Solving Hard");
			copyArray(board3, sudoku.board, 81);
		}

		time1[T1] = 0;

		if(reynoldsSolver(sudoku)){

			for(int line = 0; line < 9; line++){
				displayTextLine(line, "|%d%d%d|%d%d%d|%d%d%d|", sudoku.board[line][0],
				sudoku.board[line][1],
				sudoku.board[line][2],
				sudoku.board[line][3],
				sudoku.board[line][4],
				sudoku.board[line][5],
				sudoku.board[line][6],
				sudoku.board[line][7],
				sudoku.board[line][8]);
			}
		}
		else
			displayTextLine(0, "Error");
		displayTextLine(7, "Elapsed: %d", time1[T1]);
	}
}
