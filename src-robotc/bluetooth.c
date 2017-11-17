#include "bluetooth.h"


BT_Status establishConnection(long timeout){

	// see http://www.robotc.net/wikiarchive/NXT_Functions_Bluetooth

	// The flow is probably:
	// 1. turn on BT
	// 2. Set friendly name?? eg. Solver or Main. Probably only need to do this once (manually) since it remembers?
	// 3. set BT to visible
	// 4. btSearch?? This will add all BT devices it finds. Makes it easy to connect to them? But we should need to do this since it will remember, so we can find the other brick manually one time probably?
	// 5. btConnect(port, name) Connect to the brick nammed 'name', on the port 'port' (1,2,3).
	// Repeat until we successfully connect or we hit the timeout

	// We might have to bBTSkipPswdPrompt = true to skip entering a password when we connect???

	// Attempts to connect a BT device with a specified sFriendlyName on port nPort. nPort should be either 1, 2, or 3.
	// void btConnect(int nPort, string sFriendlyName)
}


BT_Status sendPuzzle(const Sudoku & sudoku, long timeout){

	// Use this to send the data.
	// We will need to send the puzzle in 9 messages (Each row) due to size limitations.
	// Each message which will get stored in a queue on the specified mailbox

	// Use the appropriate mailbox defined in bluetooth.h
	// Message should be an array of ubyte with size const messageLength=9
	// cCmdMessageWriteToBluetooth(mailbox, message, messageLength)
}


BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout){

	// Use the function cCmdMessageGetSize(mailbox) to determine whether any messages have been received.
	// A positive return value indicates that a message was received and is the length of the message.

	// Same as send but in reverse. Read all 9 rows.
	// We will also need to read the solved/not solved message from the other mailbox (See bluetooth.h)
	// cCmdMessageRead(mailbox, message, messageLength)
}



char * getErrorMessage(BT_Status error){
	string errorMessage = "";

	if(error == BT_ERROR)
			errorMessage = "BT Error!";
	else if (error == BT_TIMEOUT)
			errorMessage = "BT Timeout!";

		return errorMessage;
}
