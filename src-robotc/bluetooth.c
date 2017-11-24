#include "bluetooth.h"


/**
 *	Check whether a timeout has been exceeded. If timeout
 *	is negative, this function will always return false.
 *	Otherwise, if the current time exceed startTime+timeout,
 *	return true. Otherwise false.
 *
 *	param long timeout
 *			The timeout to check, in ms
 *
 *	param long startTime
 *			The time the timeout is comparing against, in ms
 *
 *	param TTimers timer
 *			The timer that startTime was measured on
 *
 *	return bool
 *			Whether of not the specified timeout has elapsed
 */
bool didTimeout(long timeout, long startTime, TTimers timer){

	// If the timeout is negative, never return true
	if(timeout < 0)
		return false;

	// If the current time is less than the endtime,
	// we haven't timed out, so return false
	if(time1[timer] < startTime+timeout)
		return false;

	// Otherwise, we've timed out. Return true
	return true;
}


/**
 *	Check whether the specified Bluetooth queue contains
 *	messages or not
 *
 *	param TMailboxIDs queue
 *			The queue to check
 *
 *	return bool
 *			Whether of not the specified queue is empty
 */
bool queueIsEmpty(TMailboxIDs queue){
	return cCmdMessageGetSize(queue) < 1;
}


/**
 *	Get the user-friendly error message for any BT_Status
 *
 *	param BT_Status status
 *			The BT_Status to parse
 *
 *	return char *
 *			The user-friendly error mesage
 */
char * getStatusMessage(BT_Status status){
	char * errorMessage = "";

	switch(status){
	case BT_SUCCESS:
		errorMessage = "Success!";
		break;
	case BT_TIMEOUT:
		errorMessage = "BT Timeout!";
		break;
	case BT_ERROR_NO_CONNECTION:
		errorMessage = "No connection";
		break;
	case BT_ERROR_READ_FAILURE:
		errorMessage = "Read Failure";
		break;
	case BT_ERROR_WRITE_FAILURE:
		errorMessage = "Write failure";
		break;
	case BT_ERROR:
	default:
		errorMessage = "Unknown BT Error";
		break;
	}

	return errorMessage;
}


/**
 *	Initialize the Bluetooth system on the brick. Set
 *	the brick's name and visibility, and turn on BT
 */
void setupBluetooth(){

	// Setup the BT parameters
	setFriendlyName(bt_myName);	// We really only need to do this once since it remembers between boots. But whatever :P
	bBTSkipPswdPrompt = true;		// Same here
	bBTHasProgressSounds = true;

	// Turn on the Bluetooth
	setBluetoothOn();
	setBluetoothVisibility(true);
}


/**
 *	Connect to this brick's peer over Bluetooth.
 *
 *	param long timeout
 *			The maximum time to wait for the peer to connect,
 *			in ms, or -1 to never timeout
 *
 *	return BT_Status
 *			BT_TIMEOUT if the timeout was exceeded before the
 *			connection was established, otherwise BT_SUCCESS
 */
BT_Status establishConnection(long timeout){
	long startTime = time1[T1];

	// *** DON'T SEARCH FOR DEVICES. PAIR THE DEVICES AHEAD OF TIME. Programatically searching is messy and unnecessary given our application ***
	// Search for nearby devices.
	//btSearch();
	//while(!(TBtDeviceStatus & btDeviceStatusNameLookedUp &&	// Wait until we find a device AND
	//				TBtDeviceStatus & btDeviceStatusAWAY) ||					// The saved device we want is found OR
	//			  !didTimeout(timeout, startTime));										// We timed out
	//btStopSearch();

	// Connect to the other brick on port 1
	// Repeat until we connect, or until we timeout
	while(nBTCurrentStreamIndex < 0 && !didTimeout(timeout, startTime)){
		btConnect(1, bt_peerName);
		wait1Msec(10);
	}

	// If we timed out, return a failure. Otherwise, we know we connected, so return success
	if(didTimeout(timeout, startTime))
		return BT_TIMEOUT;
	return BT_SUCCESS;
}


/**
 *	Send the specified puzzle as well as an isSolved bit to
 *	the paired brick.
 *
 *	param const Sudoku & sudoku
 *			The puzzle to send
 *
 *	param bool isSolved
 *			The solved/unsolved flag to send
 *
 *	return BT_Status
 *			BT_SUCCESS on successful transfer.
 *			BT_ERROR_NO_CONNECTION if there is no paired brick.
 *			Otherwise, BT_ERROR_WRITE_FAILURE
 */
BT_Status sendPuzzle(const Sudoku & sudoku, bool isSolved){

	// Empty the queues ahead of time before we send stuff.
	// Just in case there's leftover data from a previous failed transfer.
	while(!queueIsEmpty(BTQueue_metadata)){
		ubyte data[1] = {0};
		cCmdMessageRead(data, 1, BTQueue_metadata);
	}
	while(!queueIsEmpty(BTQueue_puzzleA)){
		ubyte data[1] = {0};
		cCmdMessageRead(data, 9, BTQueue_puzzleA);
	}
	while(!queueIsEmpty(BTQueue_puzzleB)){
		ubyte data[1] = {0};
		cCmdMessageRead(data, 1, BTQueue_puzzleB);
	}


	BT_Status returnStatus = BT_SUCCESS;

	// Check if we're connected to a device
	if(nBTCurrentStreamIndex < 0)
		returnStatus = BT_ERROR_NO_CONNECTION;

	// Send the rows one at a time
	const int messageLength = 9;
	ubyte puzzleRow[messageLength];
	TFileIOResult status = ioRsltSuccess;
	for(int row = 0; row < 9 && returnStatus == BT_SUCCESS; row++){

		// Read the current row from the puzzle into aa buffer
		for(int col = 0; col < messageLength; col++)
			puzzleRow[col] = sudoku[row][col];

		// Write the message. Rows 0-4 to queue A, 5-8 to B. Wait until it is complete
		if(row < 5)
			status = cCmdMessageWriteToBluetooth(puzzleRow, messageLength, BTQueue_puzzleA);
		else
			status = cCmdMessageWriteToBluetooth(puzzleRow, messageLength, BTQueue_puzzleB);
		while(status == ioRsltCommPending){
			wait1Msec(1);
			status = nBluetoothCmdStatus;
		}

		// If the message didn't send successfully, return an error
		if(status != ioRsltSuccess)
			returnStatus = BT_ERROR_WRITE_FAILURE;
	}

	// Send the true/false flag for whether or not the puzzle is solved
	if(returnStatus == BT_SUCCESS){
		ubyte data[1] = {isSolved ? 1 : 0};
		status = cCmdMessageWriteToBluetooth(data, 1, BTQueue_metadata);
		while(status == ioRsltCommPending){
			wait1Msec(1);
			status = nBluetoothCmdStatus;
		}

		// If the message didn't send successfully, return an error
		if(status != ioRsltSuccess)
			returnStatus = BT_ERROR_WRITE_FAILURE;
	}

	// Return whether or not the transfe was successful
	return returnStatus;
}


/**
 *	Wait until the paired brick sends a puzzle, then
 *	save that puzzle into the specified Sudoku and bool.
 *	If no puzzle is received within the timeout, fail.
 *
 *	param Sudoku & sudoku
 *			The puzzle to populate with the received data
 *
 *	param bool & isSolved
 *			The solved/unsolved flag to read
 *
 *	param long timeout
 *			The maximum time to wait for the paired brick to
 *			send its puzzle, in ms, or -1 to never timeout
 *
 *	return BT_Status
 *			BT_SUCCESS on successful transfer. BT_TIMEOUT on
 *			timeout. BT_ERROR_NO_CONNECTION if there is no
 *			paired brick. Otherwise, BT_ERROR_READ_FAILURE.
 */
BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout){
	long startTime = time1[T1];
	const int messageLength = 9;
	TFileIOResult status;
	BT_Status returnStatus = BT_SUCCESS;

	// Check if we're connected to a device
	if(nBTCurrentStreamIndex < 0)
		returnStatus = BT_ERROR_NO_CONNECTION;

	if(returnStatus == BT_SUCCESS){
		// Wait until we recieve a full message, or we timeout
		while((queueIsEmpty(BTQueue_metadata) || queueIsEmpty(BTQueue_puzzleA) || queueIsEmpty(BTQueue_puzzleB)) && !didTimeout(timeout, startTime));
		if(didTimeout(timeout, startTime))
			returnStatus = BT_TIMEOUT;
	}


	ubyte solved[1];

	if(returnStatus == BT_SUCCESS){
		// Read the isSolved flag
		status = cCmdMessageRead(solved, 1, BTQueue_metadata);

		// Check that we read the data properly
		if(status != ioRsltSuccess)
			returnStatus = BT_ERROR_READ_FAILURE;
	}


	// If the read was successful, save the read data
	isSolved = (bool)solved[0];

	// Read the puzzle one row at a time
	ubyte receivePuzzle[messageLength];
	for(int row = 0; row < 9 && returnStatus == BT_SUCCESS; row++){
		if(row < 5)
			status = cCmdMessageRead(receivePuzzle, messageLength, BTQueue_puzzleA);
		else
			status = cCmdMessageRead(receivePuzzle, messageLength, BTQueue_puzzleB);

		if(status != ioRsltSuccess)
			returnStatus = BT_ERROR_READ_FAILURE;
		else{
			for(int col = 0; col < messageLength; col++)
				sudoku[row][col] = receivePuzzle[col];
		}
	}

	// If we've managed to make it here, we know we read everything correctly
	return returnStatus;
}
