#ifndef BLUETOOTH_H
#define BLUETOOTH_H

enum BT_Status {
	BT_TIMEOUT,
	BT_SUCCESS,
	BT_ERROR
};

// Mailbox 1: Main->Solver [2]	{??} // Not needed??
// Mailbox 2: Main->Solver [9]	{puzzle row}
// Mailbox 3: Solver->Main [2]	{flag isSolved}
// Mailbox 4: Solver->Main [9] {puzzle row}
const TMailboxIDs bt_sendData = mailbox1;
const TMailboxIDs bt_sendPuzzle = mailbox2;
const TMailboxIDs bt_getData = mailbox3;
const TMailboxIDs bt_getPuzzle = mailbox4;


// IMPLEMENT THESE 3
BT_Status establishConnection(long timeout);
BT_Status sendPuzzle(const Sudoku & sudoku, long timeout);
BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout);

char * getErrorMessage(BT_Status error);

#endif
