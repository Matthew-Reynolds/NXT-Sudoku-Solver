#ifndef BLUETOOTH_H
#define BLUETOOTH_H

enum BT_Status {
	BT_TIMEOUT,
	BT_SUCCESS,
	BT_ERROR
};

// IMPLEMENT THESE 3
BT_Status establishConnection(long timeout);
BT_Status sendPuzzle(const Sudoku & sudoku, long timeout);
BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout);

char * getErrorMessage(BT_Status error);

#endif
