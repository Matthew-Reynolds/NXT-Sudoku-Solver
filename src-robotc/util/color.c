#include "color.h"

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
	short rgb[4];
	getColorSensorData(colorSensor, colorRaw,    &rgb[0]);

#ifdef _DEBUG
	displayTextLine(0, "%3d %3d %3d", rgb[0], rgb[1], rgb[2]);
#endif

	int cellNum = 0;

	// Red
	if(isInRGBPercent(rgb, 58, 22, 21))
		cellNum = 1;

	// Green
	else if(isInRGBPercent(rgb, 29, 48, 23))
		cellNum = 2;

	// Dark Blue
	else if(isInRGBPercent(rgb, 24, 33, 43, 3))
		cellNum = 3;

	// Yellow
	else if(isInRGBPercent(rgb, 44, 41, 15))
		cellNum = 4;

	// Light Blue
	else if(isInRGBPercent(rgb, 22, 37, 40, 3))
		cellNum = 5;

	// Pink
	else if(isInRGBPercent(rgb, 49, 20, 31))
		cellNum = 6;

	// Purple
	else if(isInRGBPercent(rgb, 29, 30, 41, 3))
		cellNum = 7;

	// Brown
	if(isInRGBPercent(rgb, 47, 30, 27))
		cellNum = 8;

	// Orange
	else if(isInRGBPercent(rgb, 56, 28, 16))
		cellNum = 9;

	return cellNum;

}
