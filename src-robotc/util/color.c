#include "color.h"


void getAvgColor(short * rgb, int readings, long interval){
	short history[3] = {0,0,0};
	long redTotal = 0;
	long greenTotal = 0;
	long blueTotal = 0;

	for(int curReading = 0; curReading < readings; curReading ++){

		// Read the R,G,B,A values into an array
		getColorSensorData(colorSensor, colorRaw, &history[0]);

		redTotal += history[0];
		greenTotal += history[1];
		blueTotal += history[2];
	}

	rgb[0] = redTotal/readings;
	rgb[1] = greenTotal/readings;
	rgb[2] = blueTotal/readings;
}


bool isInRGBPercent(const short * rgb, const short * percents, short thresh){
	return isInRGBPercent(rgb, percents[0], percents[1], percents[2], thresh);
}

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

int getCellValue(){

	// Read the R,G,B,A values into an array
	short rgb[4] = {0,0,0,0};
	//getColorSensorData(colorSensor, colorRaw,    &rgb[0]);
	getAvgColor(rgb);

#ifdef _DEBUG
	displayTextLine(0, "%3d %3d %3d", rgb[0], rgb[1], rgb[2]);
#endif

	int cellNum = 0;

	// Red
	if(isInRGBPercent(rgb, RGB_RED_THRESH))
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
	else if(isInRGBPercent(rgb, RGB_PINK_THRESH))
		cellNum = 6;

	// Purple
	else if(isInRGBPercent(rgb, RGB_PURPLE_THRESH, 3))
		cellNum = 7;

	// Brown
	else if(isInRGBPercent(rgb, RGB_BROWN_THRESH))
		cellNum = 8;

	// Orange
	else if(isInRGBPercent(rgb, RGB_ORANGE_THRESH))
		cellNum = 9;

	return cellNum;

}
