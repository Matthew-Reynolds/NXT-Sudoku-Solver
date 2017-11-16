#include "color.h"

bool isInRGBRange(const short * rgb, short minR, short minG, short minB, short maxR, short maxG, short maxB){

	short min[3] = {minR, minG, minB};
	short max[3] = {maxR, maxG, maxB};

	// Iterate through the colors R, G, and B.
	// If at any time the color falls outside the range,
	// return false.
	for(int cur = 0; cur < 3; cur++)
		if(min[cur] > rgb[cur] || rgb[cur] > max[cur]){
			displayTextLine(5, "%d < %d < %d", min[cur], rgb[cur], max[cur]);
			return false;
		}

	// Otherwise, return true
	return true;
}

int getCellValue(){


	// Read the R,G,B,A values into an array
	short rgb[4];
	getColorSensorData(colorSensor, colorRaw,    &rgb[0]);
	displayTextLine(0, "%3d %3d %3d", rgb[0], rgb[1], rgb[2]);

	int cellNum = 0;

	// Red
	if(isInRGBRange(rgb, 180,0,0, 255,120,120))
		cellNum = 1;

	// Green
	if(isInRGBRange(rgb, 0,180,0, 128,255,128))
		cellNum = 2;

	// Dark Blue
	if(isInRGBRange(rgb, 0,120,150, 150,200,255))
		cellNum = 3;

	return cellNum;

}
