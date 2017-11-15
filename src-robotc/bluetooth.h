#ifndef BLUETOOTH_H
#define BLUETOOTH_H

enum BT_Status {
	BT_TIMEOUT,
	BT_SUCCESS,
	BT_ERROR
};

BT_Status establishConnection(long timeout);
BT_Status sendPuzzle(const Sudoku & sudoku, long timeout);
BT_Status receivePuzzle(Sudoku & sudoku, long timeout);

#endif
