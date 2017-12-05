#include "test_sender.h"

/**
 *	This is a simple test program that mimics the
 *	behaviour of the primary brick to feed puzzles
 *	to the solver brick. This allows us to test the
 *	Bluetooth connection between the bricks, the
 *	solving algorithm itself, and the overall Solver.c
 *	program.
 *
 *	Author: Matthew Reynolds
 */

// Easy
Sudoku board1 =
{2, 0, 0, 0, 4, 0, 0, 3, 8,
	0, 3, 0, 0, 0, 2, 0, 1, 9,
	0, 9, 8, 3, 0, 1, 6, 2, 0,
	0, 0, 3, 2, 0, 0, 1, 0, 0,
	0, 5, 0, 0, 0, 0, 0, 6, 0,
	0, 0, 6, 0, 0, 7, 2, 0, 0,
	0, 7, 5, 4, 0, 3, 9, 8, 0,
	1, 2, 0, 7, 0, 0, 0, 5, 0,
	3, 8, 0, 0, 6, 0, 0, 0, 2};


// Medium
Sudoku board2 =
{6, 0, 0, 0, 0, 7, 0, 0, 5,
	0, 1, 0, 5, 0, 6, 0, 0, 0,
	0, 0, 9, 0, 0, 0, 7, 1, 0,
	0, 8, 0, 0, 0, 0, 3, 4, 2,
	0, 0, 0, 6, 0, 3, 0, 0, 0,
	1, 2, 3, 0, 0, 0, 0, 6, 0,
	0, 7, 1, 0, 0, 0, 6, 0, 0,
	0, 0, 0, 3, 0, 5, 0, 2, 0,
	4, 0, 0, 1, 0, 0, 0, 0, 7};


// Hard
Sudoku board3 =
{8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 3, 6, 0, 0, 0, 0, 0,
	0, 7, 0, 0, 9, 0, 2, 0, 0,
	0, 5, 0, 0, 0, 7, 0, 0, 0,
	0, 0, 0, 0, 4, 5, 7, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 3, 0,
	0, 0, 1, 0, 0, 0, 0, 6, 8,
	0, 0, 8, 5, 0, 0, 0, 1, 0,
	0, 9, 0, 0, 0, 0, 4, 0, 0};


// Print out the puzzle to the screen
void printPuzzle(const Sudoku & sudoku){
	for(int i = 0; i < 9; i++){
		displayCenteredTextLine(i, "|%d%d%d|%d%d%d|%d%d%d|",
		sudoku[i][0],
		sudoku[i][1],
		sudoku[i][2],
		sudoku[i][3],
		sudoku[i][4],
		sudoku[i][5],
		sudoku[i][6],
		sudoku[i][7],
		sudoku[i][8]);
	}
}


task main()
{
	// Setup the Bluetooth connection, mimicking
	// the primary brick
	Sudoku sudoku;
	setupBluetooth();
	establishConnection(-1);
	displayCenteredTextLine(0, "Connected!");
	displayCenteredTextLine(1, "Select puzzle");
	displayCenteredTextLine(7, "Easy  Med   Hard");

	// Repeat until the exit button is pressed.
	// This is somewhat redundant, as the firmware
	// handles this anyways, but why not
	while(nNxtButtonPressed != 0){

		// Wait until a button is pressed
		while(nNxtButtonPressed == -1);
			eraseDisplay();

		// Load the easy, medium, or hard puzzle, based
		// on the button pressed
		if(nNxtButtonPressed == 2){
			displayCenteredTextLine(0, "Sending Easy");
			copySudoku(board1, sudoku);
		}
		else if(nNxtButtonPressed == 3){
			displayCenteredTextLine(0, "Sending Med");
			copySudoku(board2, sudoku);
		}
		else if(nNxtButtonPressed == 1){
			displayCenteredTextLine(0, "Sending Hard");
			copySudoku(board3, sudoku);
		}

		// Send the puzzle
		BT_Status status = sendPuzzle(sudoku, false);
		if(status == BT_SUCCESS){
			displayCenteredTextLine(2, "Sent puzzle");

			// Wait for the response from the solver...
			bool isSolved = false;
			status = receivePuzzle(sudoku, isSolved, -1);

			// If we successfully received a response, output the received sudoku
			if(status == BT_SUCCESS){
				displayCenteredTextLine(2, "Received soln:");
				wait1Msec(500);
				printPuzzle(sudoku);
			}
			else
				displayCenteredTextLine(2, "Err: %s", getStatusMessage(status));
		}
		else
			displayCenteredTextLine(2, "Err: %s", getStatusMessage(status));
	}
}
