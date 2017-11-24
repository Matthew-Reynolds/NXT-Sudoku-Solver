#include "color.h"


/**
 *	Determine the RGB values of the color sensor over an
 *	average of the specified number of readings taken at
 *	the specified interval
 *
 *	param short * rgb
 *			The array of shorts into which to store the rgb data
 *			(Format: 0=red, 1=green, 2=blue, 3=ambient)
 *
 *	param int readings
 *			The number of readings to take (Default: 5)
 *
 *	param long interval
 *			The timeout between readings, in ms (Default: 10)
 */
void getAvgColor(short * rgb, int readings, long interval){
	short history[4] = {0,0,0, 0};
	long redTotal = 0;
	long greenTotal = 0;
	long blueTotal = 0;
	long ambientTotal = 0;

	for(int curReading = 0; curReading < readings; curReading ++){

		// Read the R,G,B,A values into an array
		getColorSensorData(colorSensor, colorRaw, &history[0]);

		redTotal += history[0];
		greenTotal += history[1];
		blueTotal += history[2];
		ambientTotal += history[3];
	}

	rgb[0] = redTotal/readings;
	rgb[1] = greenTotal/readings;
	rgb[2] = blueTotal/readings;
	rgb[3] = ambientTotal/readings;
}


/**
 *	Determine whether the specified color is approximately
 *	equal to the specified values. Compare using relative
 *	percentages of color rather than the raw RGB values
 *	(eg. redVal = rgb[0] / (rgb[0]+rgb[1]+rgb[2])
 *
 *	param const short * rgb
 *			The RGB value to be compared
 *
 *	param const short * percents
 *			The RGB value to compare against, in percentages
 *
 *	param short thresh
 *			The allowable deviation between the two values in order
 *			for them to be considered equal, in percent (Default: 5)
 *
 *	return bool
 *			Whether or not the specified color is equal to the
 *			value
 */
bool isInRGBPercent(const short * rgb, const short * percents, short thresh){
	return isInRGBPercent(rgb, percents[0], percents[1], percents[2], thresh);
}


/**
 *	Determine whether the specified color is approximately
 *	equal to the specified values. Compare using relative
 *	percentages of color rather than the raw RGB values
 *	(eg. redVal = rgb[0] / (rgb[0]+rgb[1]+rgb[2])
 *
 *	param const short * rgb
 *			The RGB value to be compared
 *
 *	param short red
 *			The red value to compare against, in percentage
 *
 *	param short green
 *			The green value to compare against, in percentage
 *
 *	param short blue
 *			The blue value to compare against, in percentage
 *
 *	param short thresh
 *			The allowable deviation between the two values in order
 *			for them to be considered equal, in percent (Default: 5)
 *
 *	return bool
 *			Whether or not the specified color is equal to the
 *			value
 */
bool isInRGBPercent(const short * rgb, short red, short green, short blue, short thresh){

	// Handle divide by 0
	if(rgb[0]+rgb[1]+rgb[2] == 0)
		return false;

	// Calculate the percentages
	short percent[3] = {0,0,0};

	percent[0] = (float)(rgb[0])*100/(rgb[0]+rgb[1]+rgb[2]);
	percent[1] = (float)(rgb[1])*100/(rgb[0]+rgb[1]+rgb[2]);
	percent[2] = (float)(rgb[2])*100/(rgb[0]+rgb[1]+rgb[2]);


#ifdef _DEBUG
	displayTextLine(5, "%d %d %d", percent[0], percent[1], percent[2]);
#endif


	// Check if the read value is outside the range for each red, green, and blue
	bool isInRange = true;
	if(red+thresh < percent[0] || red-thresh > percent[0])
		isInRange = false;
	if(green+thresh < percent[1] || green-thresh > percent[1])
		isInRange = false;
	if(blue+thresh < percent[2] || blue-thresh > percent[2])
		isInRange = false;

	// Return whether or not the read value was in the specified range
	return isInRange;
}


/**
 *	Determine whether the specified color is approximately
 *	equal to the specified values. Compare using raw RGB values
 *
 *	param const short * rgb
 *			The RGB value to be compared
 *
 *	param short minR
 *			The lower bound for the red value
 *
 *	param short minR
 *			The upper bound for the red value
 *
 *	param short minG
 *			The lower bound for the green value
 *
 *	param short minG
 *			The upper bound for the green value
 *
 *	param short minB
 *			The lower bound for the blue value
 *
 *	param short minB
 *			The upper bound for the blue value
 *
 *	return bool
 *			Whether or not the specified color is within the
 *			specified bounds
 */
bool isInRGBRange(const short * rgb, short minR, short minG, short minB, short maxR, short maxG, short maxB){

	short min[3] = {minR, minG, minB};
	short max[3] = {maxR, maxG, maxB};

	bool isInRange = true;

	// Iterate through the colors R, G, and B.
	// If at any time the color falls outside the range,
	// return false.
	for(int cur = 0; cur < 3; cur++)
		if(min[cur] > rgb[cur] || rgb[cur] > max[cur]){
		isInRange = false;

#ifdef _DEBUG
		displayTextLine(5, "%d < %d < %d", min[cur], rgb[cur], max[cur]);
#endif
	}

	// Otherwise, return true
	return isInRange;
}


/**
 *	Determine the cell value below the color sensor by
 *	comparing the background color of the cell to a known
 *	color representing each value 0-9
 *
 *	return int
 *			The read cell value, or -1 if no value was read
 */
int getCellValue(){

	// Read the R,G,B,A values into an array
	short rgb[4] = {0,0,0,0};
	getAvgColor(rgb);

#ifdef _DEBUG
	displayTextLine(0, "%3d %3d %3d", rgb[0], rgb[1], rgb[2]);
#endif

	int cellNum = -1;

	// White
	if(isInRGBPercent(rgb, RGB_WHITE_THRESH))
		cellNum = 0;

	// Red
	else if(isInRGBPercent(rgb, RGB_RED_THRESH, 3))
		cellNum = 1;

	// Green
	else if(isInRGBPercent(rgb, RGB_GREEN_THRESH))
		cellNum = 2;

	// Dark Blue
	else if(isInRGBPercent(rgb, RGB_DBLUE_THRESH, 3))
		cellNum = 3;

	// Yellow
	else if(isInRGBPercent(rgb, RGB_YELLOW_THRESH))
		cellNum = 4;

	// Light Blue
	else if(isInRGBPercent(rgb, RGB_LBLUE_THRESH, 3))
		cellNum = 5;

	// Pink
	else if(isInRGBPercent(rgb, RGB_PINK_THRESH, 3))
		cellNum = 6;

	// Purple
	else if(isInRGBPercent(rgb, RGB_PURPLE_THRESH, 3))
		cellNum = 7;

	// Brown
	else if(isInRGBPercent(rgb, RGB_BROWN_THRESH, 3))
		cellNum = 8;

	// Orange
	else if(isInRGBPercent(rgb, RGB_ORANGE_THRESH, 3))
		cellNum = 9;

	return cellNum;
}
