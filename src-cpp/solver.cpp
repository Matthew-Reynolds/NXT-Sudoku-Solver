#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

inline short decToBit(short dec){
	return 1<<dec-1;
	//return (short) pow(2, dec-1);	
}

inline short bitToDec(short bit){
	if(bit < 1)
		return 0;
	
	return int(log(bit)/log(2)) + 1;	
}

inline bool isSingleBit(short bit){
	if(bit < 1)
		return false;
	return fmod(log(bit)/log(2), 1) < 0.01;	
}

inline short getBoxNumber(short row, short col){
	return int(col/3) + 3*(row/3);
}

inline short getBoxEntryCol(short box, short entry){
	return 3*(box%3) + (entry%3);
}

inline short getBoxEntryRow(short box, short entry){
	return 3*(box/3) + (entry/3);
}

void copyPuzzle(short source[][9], short dest[][9]){
	for(int row = 0; row < 9; row++){
		for(int col = 0; col < 9; col++){
			dest[row][col] = source[row][col];
		}
	}
}

void copyArray(short source[], short dest[], int size){
	for(int cur = 0; cur < size; cur++)
		dest[cur] = source[cur];
}

void printPuzzle(const short board[][9]){

	cout << "\n\n\n";

	for(short i = 0; i < 13; i++){
		
		for(short j = 0; j < 19; j++){
			if(i%4 == 0)
				cout << "-";
			else{
				if(j%6 == 0)
					cout << "|";
				else if (j % 2 == 0)
					cout << " ";
				else{
					short row = i-(i/4) - 1;
					if(board[row][j/2] != 0)
						cout << board[row][j/2];
					else
						cout << " ";
				}
			}
			
		}
		cout << endl;
	}
	
}
bool checkValid(const short board[][9]){
	bool isValid = true;
	
	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0};
		short colInstances[9] = {0};
		short boxInstances[9] = {0};
		
		for(int minor = 0; minor < 9; minor++){
			short val = 0;
			
			if(val = board[major][minor])
				rowInstances[val-1]++;
			if(val = board[minor][major])
				colInstances[val-1]++;
			if(val = board[getBoxEntryRow(major, minor)]
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

bool checkSolved(const short board[][9]){
	bool isValid = true;
	
	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0};
		short colInstances[9] = {0};
		short boxInstances[9] = {0};
		
		for(int minor = 0; minor < 9; minor++){
			short val = 0;
			
			if(val = board[major][minor])
				rowInstances[val-1]++;
			if(val = board[minor][major])
				colInstances[val-1]++;
			if(val = board[getBoxEntryRow(major, minor)]
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


// Attempt to combine the other three functions. This fails, because it doesn't
// update each of numRowInstances, Col, and Box each time it finds a val
/*bool checkAll(short potentialColVals[],
			  short potentialRowVals[],
			  short potentialBoxVals[],
			  short board[][9]){
			  	
	bool madeChange = false;
	
	// Iterate through every col/row/box
	for(short major = 0; major < 9; major++){
		short numRowInstances[9][2] = {0};
		short numColInstances[9][2] = {0};
		short numBoxInstances[9][2] = {0};
		
		for(short minor = 0; minor < 9; minor++){
			
			// *** ROW
			if(board[major][minor] < 1){
				short newBit = potentialColVals[minor] &
							   potentialRowVals[major] &
							   potentialBoxVals[getBoxNumber(major, minor)];
							   
				for(short bit = 0; bit < 9; bit++)
					if(newBit & decToBit(bit+1)){
						numRowInstances[bit][0] ++;
						numRowInstances[bit][1] = minor;
					}	
			}
			
			// *** COL
			if(board[minor][major] < 1){
				short newBit = potentialColVals[major] &
							   potentialRowVals[minor] &
							   potentialBoxVals[getBoxNumber(minor, major)];
							   
				for(short bit = 0; bit < 9; bit++)
					if(newBit & decToBit(bit+1)){
						numColInstances[bit][0] ++;
						numColInstances[bit][1] = minor;
					}	
			}
			
			// *** BOX
			if(board[getBoxEntryRow(major, minor)][getBoxEntryCol(major, minor)] < 1){
				short newBit = potentialColVals[getBoxEntryCol(major, minor)] &
							   potentialRowVals[getBoxEntryRow(major, minor)] &
							   potentialBoxVals[major];
							   
				for(short bit = 0; bit < 9; bit++)
					if(newBit & decToBit(bit+1)){
						numBoxInstances[bit][0] ++;
						numBoxInstances[bit][1] = minor;
					}	
			}
		}
			
		// Now that we've counted the possible locations, find all the values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			
			// ***ROW
			if(numRowInstances[curVal][0] == 1){
				madeChange = true;
				board[major][numRowInstances[curVal][1]] = curVal+1;
				
				potentialColVals[numRowInstances[curVal][1]] -= decToBit(curVal+1);
				potentialRowVals[major] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(major, numRowInstances[curVal][1])] -= decToBit(curVal+1);
			}
				
			// ***COL
			if(numColInstances[curVal][0] == 1){
				madeChange = true;
				board[numColInstances[curVal][1]][major] = curVal+1;
				
				potentialColVals[major] -= decToBit(curVal+1);
				potentialRowVals[numColInstances[curVal][1]] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(numColInstances[curVal][1], major)] -= decToBit(curVal+1);
			}
			
			// ***BOX
			if(numBoxInstances[curVal][0] == 1){
				madeChange = true;
				board[getBoxEntryRow(major, numBoxInstances[curVal][1])]
					 [getBoxEntryCol(major, numBoxInstances[curVal][1])]
					 = curVal+1;
				
				potentialColVals[getBoxEntryCol(major, numBoxInstances[curVal][1])] -= decToBit(curVal+1);
				potentialRowVals[getBoxEntryRow(major, numBoxInstances[curVal][1])] -= decToBit(curVal+1);
				potentialBoxVals[major] -= decToBit(curVal+1);
			}
		}
	}
	return madeChange; 	
}
*/


bool checkCol(short potentialColVals[],
			  short potentialRowVals[],
			  short potentialBoxVals[],
			  short board[][9]){
		
	bool madeChange = false;
		
	// Iterate through each col, one at a time
	for(short col = 0; col < 9; col++){
		short numInstances[9] = {0};
		short possibleRow[9] = {0};
		
		// Iterate through every number 1-9, and get how many spots this number can be placed in this column
		for(short row = 0; row < 9; row++){
			if(board[row][col] < 1){
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
				board[possibleRow[curVal]][col] = curVal+1;
				
				potentialColVals[col] -= decToBit(curVal+1);
				potentialRowVals[possibleRow[curVal]] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(possibleRow[curVal], col)] -= decToBit(curVal+1);
			}
		}
	}
	
	return madeChange;
}

bool checkRow(short potentialColVals[],
			  short potentialRowVals[],
			  short potentialBoxVals[],
			  short board[][9]){
		
	bool madeChange = false;
		
	// Iterate through each row, one at a time
	for(short row = 0; row < 9; row++){
		
		short numInstances[9] = {0};
		short possibleCol[9] = {0};
		
	// Iterate through every number 1-9, and get how many spots this number can be placed in this row
	for(short col = 0; col < 9; col++){
		if(board[row][col] < 1){
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
				board[row][possibleCol[curVal]] = curVal+1;
				
				potentialColVals[possibleCol[curVal]] -= decToBit(curVal+1);
				potentialRowVals[row] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(row, possibleCol[curVal])] -= decToBit(curVal+1);
			}
		}
	}
	
	return madeChange;
}

bool checkBox(short potentialColVals[],
			  short potentialRowVals[],
			  short potentialBoxVals[],
			  short board[][9]){
		
	bool madeChange = false;
		
	// Iterate through each box, one at a time
	for(short box = 0; box < 9; box++){
		short numInstances[9] = {0};
		short possibleEntry[9] = {0};
		
		// Iterate through every entry 1-9, and get how many spots this number can be placed in this row
		for(short entry = 0; entry < 9; entry++){
			if(board[getBoxEntryRow(box, entry)][getBoxEntryCol(box, entry)] < 1){
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
				board[getBoxEntryRow(box, possibleEntry[curVal])]
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



bool reynoldsSolverInner(short potentialColVals[],
						 short potentialRowVals[],
						 short potentialBoxVals[],
						 short board[][9]){

	short saveBoard[9][9];
	short saveColVals[9];
	short saveRowVals[9];
	short saveBoxVals[9];
	copyPuzzle(board, saveBoard);
	copyArray(potentialColVals, saveColVals, 9);
	copyArray(potentialRowVals, saveRowVals, 9);
	copyArray(potentialBoxVals, saveBoxVals, 9);
	
	cout << "\n\n\n =========== LAYER ==============";

		printPuzzle(board);

	// Make as many changes using logic as possible
	bool madeChange = false;
	do{	
		madeChange = checkCol(potentialColVals, potentialRowVals, potentialBoxVals, board);		
		madeChange |= checkRow(potentialColVals, potentialRowVals, potentialBoxVals, board);
		madeChange |= checkBox(potentialColVals, potentialRowVals, potentialBoxVals, board);
		printPuzzle(board);
	} while(madeChange);
	
	
	// Check if we've solved it
	bool isSolved = checkSolved(board);
	
	// If logic can't get us any further and the puzzle still isn't solved, we
	// need to try a little bit of trial-&-error / backtracking.
		
		
	bool gotCell = false;
	
	// Find the first empty cell
	for(short row = 0; row < 9 && !isSolved && !gotCell; row++){
		for(short col = 0; col < 9 && !isSolved && !gotCell; col++){
			if(board[row][col] == 0){
				gotCell = true;
				
				// Get the potential vals that can go here
				short potVals = potentialColVals[col] &
								potentialRowVals[row] &
								potentialBoxVals[getBoxNumber(row, col)];
								
				// For each potential val...
				for(short curVal = 1; curVal <= 9 && !isSolved; curVal++){
					if(decToBit(curVal) & potVals){
						
						// Sub in the val and RECURSION	
						board[row][col] = curVal;
						cout << "(" << potentialRowVals[row];
						potentialRowVals[row] -= decToBit(curVal);
						potentialColVals[col] -= decToBit(curVal);
						potentialBoxVals[getBoxNumber(row, col)] -= decToBit(curVal);
						
						cout << ") subbing in a " << curVal << "to C" << col << " R" << row;
						cout << "(" << potentialRowVals[row] << endl;
						
						printPuzzle(board);
						
						isSolved = reynoldsSolverInner(potentialColVals, potentialRowVals, potentialBoxVals, board);
						cout << "\n\n\n =========== OUT ==============";
						
						// If it's solved, great! Exit. If not, reset this val and try the next one.
						if(!isSolved){
							board[row][col] = 0;
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
		copyPuzzle(saveBoard, board);
		copyArray(saveColVals, potentialColVals, 9);
		copyArray(saveRowVals, potentialRowVals, 9);
		copyArray(saveBoxVals, potentialBoxVals, 9);
		
	}
	
	return isSolved;					 	
}

bool reynoldsSolver(short board[][9]){

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
			short cellBit = decToBit(board[row][col]);
			potentialColVals[col] -= cellBit;
			potentialRowVals[row] -= cellBit;
			potentialBoxVals[getBoxNumber(row, col)] -= cellBit;
		}
	}
	
	bool result = reynoldsSolverInner(potentialColVals,
									  potentialRowVals,
									  potentialBoxVals,
									  board);
	
	return result;
}


// An innefficient but simple recursive brute-force solver.
bool recursiveSolver(short board[][9]){
	bool isSolved = false;
	bool foundBlankCell = false;
	
	// Find the first empty cell
	for(short row = 0; row < 9 && !isSolved && !foundBlankCell; row ++){
		for(short col = 0; col < 9 && !isSolved && !foundBlankCell; col++){
			if(board[row][col] < 1){
				foundBlankCell = true;
				
				// Try the values 1-9 in this cell
				for(short i = 1; i <= 9 && !isSolved; i++){
					bool isValid = true;
					
					// Check col
					for(short curRow = 0; curRow < 9; curRow ++)
						if(board[curRow][col] == i)
							isValid = false;
							
					// Check row
					for(short curCol = 0; curCol < 9; curCol ++)
						if(board[row][curCol] == i)
							isValid = false;					
						
					// Check box
					short boxColOrig = 3*(col/3);
					short boxRowOrig = 3*(row/3);
					for(short curRow =boxRowOrig; curRow < boxRowOrig + 3; curRow ++){
						for(short curCol =boxColOrig; curCol < boxColOrig + 3; curCol ++){
							if(board[curRow][curCol] == i)
								isValid = false;
						}	
					}
					
					// We don't know whether or not it's the correct first value,
					// but we have found the first possible value.
					if(isValid){
						
						// Sub in the value...
						board[row][col] = i;
						
						// And repeat the process on the next cell.
						isSolved = recursiveSolver(board);
						
						// If it turns out that it's impossible to solve the puzzle
						// given value we put in the cell, reset the value.
						if(!isSolved)
							board[row][col] = 0;
						
					}
					
					
				} // End of testing vals
				
				// At this point, based on isSolved, we know whether a potential
				// value was found or if there's no possible legal value for 
				// this cell
					
			} // End of if (cell is emtpy)
		} // End of col loop
	} // End of row loop
	
	// If isSolved is true, we know the current value we have is legal.
	// If we found no blank cells, we know the puzzle is solved.
	return isSolved || !foundBlankCell;
}

int main(){
	
	
	// Easy
//	short board[9][9] =
//		 {{2, 0, 0, 0, 4, 0, 0, 3, 8},
//		//{{1, 0, 0, 0, 4, 0, 0, 3, 8},
//		 {0, 3, 0, 0, 0, 2, 0, 1, 0},
//		 {0, 9, 8, 3, 0, 1, 6, 2, 0},
//		 {0, 0, 3, 2, 0, 0, 1, 0, 0},
//		 {0, 5, 0, 0, 0, 0, 0, 6, 0},
//		 {0, 0, 6, 0, 0, 7, 2, 0, 0},
//		 {0, 7, 5, 4, 0, 3, 9, 8, 0},
//		 {1, 2, 0, 7, 0, 0, 0, 5, 0},
//		 {3, 8, 0, 0, 6, 0, 0, 0, 0}};

	// Medium
	short board[9][9] =
		 {{6, 0, 0, 0, 0, 7, 0, 0, 5},
		  {0, 1, 0, 5, 0, 6, 0, 0, 0},
		  {0, 0, 9, 0, 0, 0, 7, 1, 0},
		  {0, 8, 0, 0, 0, 0, 3, 4, 2},
		  {0, 0, 0, 6, 0, 3, 0, 0, 0},
		  {1, 2, 3, 0, 0, 0, 0, 6, 0},
		  {0, 7, 1, 0, 0, 0, 6, 0, 0},
		  {0, 0, 0, 3, 0, 5, 0, 2, 0},
		  {4, 0, 0, 1, 0, 0, 0, 0, 7}};
		 
//	// Hard
//	short board[9][9] =
//		 {{8, 0, 0, 0, 0, 0, 0, 0, 0},
//		  {0, 0, 3, 6, 0, 0, 0, 0, 0},
//		  {0, 7, 0, 0, 9, 0, 2, 0, 0},
//		  {0, 5, 0, 0, 0, 7, 0, 0, 0},
//		  {0, 0, 0, 0, 4, 5, 7, 0, 0},
//		  {0, 0, 0, 1, 0, 0, 0, 3, 0},
//		  {0, 0, 1, 0, 0, 0, 0, 6, 8},
//		  {0, 0, 8, 5, 0, 0, 0, 1, 0},
//		  {0, 9, 0, 0, 0, 0, 4, 0, 0}};
	
	
//	printPuzzle(board);	
	cout << (checkValid(board) ? "Valid" : "Invalid");
	
	cout << (reynoldsSolver(board) ? "Solved!" : "Not solveable :(");
		
	printPuzzle(board);
}
/*
-------------------
|2 6 1|5 4 9|7 3 8|
|4 3 7|6 8 2|5 1 9|
|5 9 8|3 7 1|6 2 4|
-------------------
|8 4 3|2 5 6|1 9 7|
|7 5 2|9 1 4|8 6 3|
|9 1 6|8 3 7|2 4 5|
-------------------
|6 7 5|4 2 3|9 8 1|
|1 2 4|7 9 8|3 5 6|
|3 8 9|1 6 5|4 7 2|
-------------------
*/
