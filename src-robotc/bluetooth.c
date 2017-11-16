#include "bluetooth.h"


BT_Status establishConnection(long timeout){
	// Code me!!
}


BT_Status sendPuzzle(const Sudoku & sudoku, long timeout){
	// Code me!!
}


BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout){
	// Code me!!
}



char * getErrorMessage(BT_Status error){
	string errorMessage = "";

	if(error == BT_ERROR)
			errorMessage = "BT Error!";
	else if (error == BT_TIMEOUT)
			errorMessage = "BT Timeout!";

		return errorMessage;
}
