#include "test_color.h"


/**
 *	This is a simple test program used to validate the
 *	color sensor fuctionality as well as test/recalibrate
 *	the thresholds.
 *
 *	This program simply continually outputs the R, G, and B
 *	color readings, as well as the corresponding threshold,
 *	if any.
 *
 *	Author: Matthew Reynolds
 */
task main()
{

	// Setup the color sensor
	SensorType[colorSensor] = sensorColorNxtFULL;

	// Loop until a button is pressed
	while(nNxtButtonPressed == -1)
		displayCenteredBigTextLine(1, "Val: %d", getCellValue());

}
