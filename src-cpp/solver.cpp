#include <cmath>
#include <iostream>

using namespace std;

inline int getBoxNumber(int row, int col){
	return int(col/3) + 1 + 3*(row/3);
}


void printPuzzle(int board[][9]){

	cout << "\n\n\n";

	for(int i = 0; i < 13; i++){
		
		for(int j = 0; j < 19; j++){
			if(i%4 == 0)
				cout << "-";
			else{
				if(j%6 == 0)
					cout << "|";
				else if (j % 2 == 0)
					cout << " ";
				else{
					int row = i-(i/4) - 1;
					//int col = j/2;
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

// An innefficient but simple recursive brute-force solver.
bool recursiveSolver(int board[][9]){
	bool isSolved = false;
	bool foundBlankCell = false;
	
	// Find the first empty cell
	for(int row = 0; row < 9 && !isSolved && !foundBlankCell; row ++){
		for(int col = 0; col < 9 && !isSolved && !foundBlankCell; col++){
			if(board[row][col] < 1){
				foundBlankCell = true;
				
				// Try the values 1-9 in this cell
				for(int i = 1; i <= 9 && !isSolved; i++){
					bool isValid = true;
					
					// Check col
					for(int curRow = 0; curRow < 9; curRow ++)
						if(board[curRow][col] == i)
							isValid = false;
							
					// Check row
					for(int curCol = 0; curCol < 9; curCol ++)
						if(board[row][curCol] == i)
							isValid = false;					
						
					// Check box
					int boxColOrig = 3*(col/3);
					int boxRowOrig = 3*(row/3);
					for(int curRow =boxRowOrig; curRow < boxRowOrig + 3; curRow ++){
						for(int curCol =boxColOrig; curCol < boxColOrig + 3; curCol ++){
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
	int board[9][9] =
		 {{2, 0, 0, 0, 4, 0, 0, 3, 8},
		//{{1, 0, 0, 0, 4, 0, 0, 3, 8},
		 {0, 3, 0, 0, 0, 2, 0, 1, 9},
		 {0, 9, 8, 3, 0, 1, 6, 2, 0},
		 {0, 0, 3, 2, 0, 0, 1, 0, 0},
		 {0, 5, 0, 0, 0, 0, 0, 6, 0},
		 {0, 0, 6, 0, 0, 7, 2, 0, 0},
		 {0, 7, 5, 4, 0, 3, 9, 8, 0},
		 {1, 2, 0, 7, 0, 0, 0, 5, 0},
		 {3, 8, 0, 0, 6, 0, 0, 0, 2}};
		 
	// Hard
//	int board[9][9] =
//		 {{8, 0, 0, 0, 0, 0, 0, 0, 0},
//		  {0, 0, 3, 6, 0, 0, 0, 0, 0},
//		  {0, 7, 0, 0, 9, 0, 2, 0, 0},
//		  {0, 5, 0, 0, 0, 7, 0, 0, 0},
//		  {0, 0, 0, 0, 4, 5, 7, 0, 0},
//		  {0, 0, 0, 1, 0, 0, 0, 3, 0},
//		  {0, 0, 1, 0, 0, 0, 0, 6, 8},
//		  {0, 0, 8, 5, 0, 0, 0, 1, 0},
//		  {0, 9, 0, 0, 0, 0, 4, 0, 0}};
		 
	printPuzzle(board);	
	
	cout << (recursiveSolver(board) ? "Solved!" : "Not solveable :(");
		
	printPuzzle(board);
	
}
