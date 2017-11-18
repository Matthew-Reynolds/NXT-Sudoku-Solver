#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// Declare a series of states that can occur when
// executing our Bluetooth commands
enum BT_Status {
	BT_SUCCESS = 0x00,
	BT_TIMEOUT = 0x20,
	BT_ERROR = 0x10,
	BT_ERROR_NO_CONNECTION = BT_ERROR | 0x01,
	BT_ERROR_WRITE_FAILURE = BT_ERROR | 0x02,
	BT_ERROR_READ_FAILURE = BT_ERROR | 0x04
};

// Note that the puzzle is transmitted in two parts due to size limitations of the mailboxes.
// Each mailbox can only hold 58 bytes, while the puzzle is 81 bytes. Therefore, we send
// rows 0-4 of the puzzle in BTQueue_puzzleA and rows 5-8 in B.
const TMailboxIDs BTQueue_metadata = mailbox1;
const TMailboxIDs BTQueue_puzzleA = mailbox2;
const TMailboxIDs BTQueue_puzzleB = mailbox3;

// Declare the friendly name of this brick and the other brick
// (To be used when establishing the Bluetooth connection)
#ifdef BRICK_PRIMARY
	string bt_myName = "Primary";
	const string bt_peerName = "Solver";
#elif defined(BRICK_SOLVER)
	string bt_myName = "Solver";
	const string bt_peerName = "Primary";
#endif


// Helper functions
bool didTimeout(long timeout, long startTime);
bool queueIsEmpty(TMailboxIDs queue);
char * getStatusMessage(BT_Status status);

// Main bluetooth functions
void setupBluetooth();
BT_Status establishConnection(long timeout);
BT_Status sendPuzzle(const Sudoku & sudoku, bool isSolved);
BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout);

#endif
