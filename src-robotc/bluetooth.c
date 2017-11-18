#include "bluetooth.h"

bool didTimeout(long timeout, long startTime){

	// If the timeout is negative, never return true
	if(timeout < 0)
		return false;

	// If the current time is less than the endtime,
	// we haven't timed out, so return false
	if(time1[T1] < startTime+timeout)
		return false;

	// Otherwise, we've timed out. Return true
	return true;
}


bool queueIsEmpty(TMailboxIDs queue){
	return cCmdMessageGetSize(queue) < 1;
}


void setupBluetooth(){

	// Set the name of this brick
	setFriendlyName(bt_myName);	// We really only need to do this once since it remembers between boots. But whatever :P
	bBTSkipPswdPrompt = true;		// Same here


	// Turn on the Bluetooth
	setBluetoothOn();
	bBTHasProgressSounds = true;
	setBluetoothVisibility(true);
}



BT_Status establishConnection(long timeout){
	long startTime = time1[T1];

	// *** DON'T SEARCH FOR DEVICES. PAIR THE DEVICES AHEAD OF TIME. Programatically searching is messy and unecessary given our application ***
	// Search for nearby devices.
	//btSearch();
	//while(!(TBtDeviceStatus & btDeviceStatusNameLookedUp &&	// Wait until we find a device AND
	//				TBtDeviceStatus & btDeviceStatusAWAY) ||					// The saved device we want is found OR
	//			  !didTimeout(timeout, startTime));										// We timed out
	//btStopSearch();


	// Connect to "Solver" on port 1
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




BT_Status sendPuzzle(const Sudoku & sudoku, bool isSolved){

	//Empty the queues ahead of time before we send stuff. Just in case there's leftover data from last time.
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

	// Check if we're connected to a device
	if(nBTCurrentStreamIndex < 0)
		return BT_ERROR_NO_CONNECTION;



	// Send the rows one at a time
	const int messageLength = 9;
	ubyte puzzleRow[messageLength];
	TFileIOResult status = ioRsltSuccess;
	for(int row = 0; row < 9; row++){

		// Read the current row from the puzzle into aa buffer
		for(int col = 0; col < 9; col++)
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
			return BT_ERROR_WRITE_FAILURE;
	}

	// Send the true/false flag for whether or not the puzzle is solved
	ubyte data[1] = {isSolved};
	status = cCmdMessageWriteToBluetooth(data, 1, BTQueue_metadata);
	while(status == ioRsltCommPending){
		wait1Msec(1);
		status = nBluetoothCmdStatus;
	}

	// If the message didn't send successfully, return an error
	if(status != ioRsltSuccess)
		return BT_ERROR_WRITE_FAILURE;

	return BT_SUCCESS;
}


BT_Status receivePuzzle(Sudoku & sudoku, bool & isSolved, long timeout){
	long startTime = time1[T1];
	const int messageLength = 9;
	TFileIOResult status;

	// Check if we're connected to a device
	if(nBTCurrentStreamIndex < 0)
		return BT_ERROR_NO_CONNECTION;

	// Wait until we recieve a full message, or we timeout
	while((queueIsEmpty(BTQueue_metadata) || queueIsEmpty(BTQueue_puzzleA) || queueIsEmpty(BTQueue_puzzleB)) && !didTimeout(timeout, startTime));
	if(didTimeout(timeout, startTime))
		return BT_TIMEOUT;

	// Read the isSolved flag
	ubyte solved[1];
	status = cCmdMessageRead(solved, 1, BTQueue_metadata);

	// Check that we read the data properly
	if(status != ioRsltSuccess)
		return BT_ERROR_READ_FAILURE;

	// If the read was successful, save the read data
	isSolved = (bool)solved[0];

	// Read the puzzle one row at a time
	ubyte receivePuzzle[messageLength];
	for(int row = 0; row < messageLength; row++){
		if(row < 5)
			status = cCmdMessageRead(receivePuzzle, messageLength, BTQueue_puzzleA);
		else
			status = cCmdMessageRead(receivePuzzle, messageLength, BTQueue_puzzleB);
		if(status != ioRsltSuccess)
			return BT_ERROR;

		for(int col = 0; col < messageLength; col++)
			sudoku[row][col] = receivePuzzle[col];
	}

	// If we've managed to make it here, we know we read everything correctly
	return BT_SUCCESS;
}



char * getStatusMessage(BT_Status status){
	char * errorMessage = "aaaa";

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
