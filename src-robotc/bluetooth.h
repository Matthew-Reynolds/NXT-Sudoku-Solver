#ifndef BLUETOOTH_H
#define BLUETOOTH_H


enum BT_Status {
	BT_SUCCESS = 0x00,
	BT_TIMEOUT = 0x20,
	BT_ERROR = 0x10,
	BT_ERROR_NO_CONNECTION = BT_ERROR | 0x01,
	BT_ERROR_WRITE_FAILURE = BT_ERROR | 0x02,
	BT_ERROR_READ_FAILURE = BT_ERROR | 0x04
};

// Mailbox 1: Main->Solver [2]	{??} // Not needed??
// Mailbox 2: Main->Solver [9]	{puzzle row}
// Mailbox 3: Solver->Main [2]	{flag isSolved}
// Mailbox 4: Solver->Main [9] {puzzle row}

const TMailboxIDs BTQueue_metadata = mailbox1;
const TMailboxIDs BTQueue_puzzleA = mailbox2;
const TMailboxIDs BTQueue_puzzleB = mailbox3;

#ifdef BRICK_PRIMARY
string bt_myName = "Primary";
const string bt_peerName = "Solver";
#elif defined(BRICK_SOLVER)
string bt_myName = "Solver";
const string bt_peerName = "Primary";

#endif


bool didTimeout(long timeout, long startTime);
bool queueIsEmpty(TMailboxIDs queue);

void setupBluetooth();
BT_Status establishConnection(long timeout);
BT_Status sendPuzzle(const Sudoku & sudoku, bool isSolved);
BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout);

char * getStatusMessage(BT_Status status);

#endif
