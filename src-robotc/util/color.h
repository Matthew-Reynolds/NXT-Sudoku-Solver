#ifndef COLOR_H
#define COLOR_H

void getAvgColor(short * rgb, int readings = 5, long interval = 10);
bool isInRGBPercent(const short * rgb, const short * percents, short thresh = 5);
bool isInRGBPercent(const short * rgb, short red, short green, short blue, short thresh = 5);
bool isInRGBRange(const short * rgb, short minR, short minG, short minB, short maxR, short maxG, short maxB);
int getCellValue();



// LOAD THE CONSTANTS FOR THE RGB THRESHOLDS
//#define LOCATION_SJU
#define LOCATION_WATIMAKE
//#define LOCATION_WEEF

#ifdef LOCATION_SJU
const short RGB_RED_THRESH[3] = 		{58, 22, 21};
const short RGB_GREEN_THRESH[3] =		{29, 48, 23};
const short RGB_DBLUE_THRESH[3] =		{24, 33, 43};
const short RGB_YELLOW_THRESH[3] =	{44, 41, 15};
const short RGB_LBLUE_THRESH[3] =		{22, 37, 40};
const short RGB_PINK_THRESH[3] =		{49, 20, 31};
const short RGB_PURPLE_THRESH[3] =	{29, 30, 41};
const short RGB_BROWN_THRESH[3] =		{47, 30, 23};
const short RGB_ORANGE_THRESH[3] =	{56, 28, 16};

#elif defined(LOCATION_WATIMAKE)
const short RGB_RED_THRESH[3] = 		{64, 19, 17};
const short RGB_GREEN_THRESH[3] =		{26, 52, 22};
const short RGB_DBLUE_THRESH[3] =		{22, 35, 43};
const short RGB_YELLOW_THRESH[3] =	{46, 43, 10};
const short RGB_LBLUE_THRESH[3] =		{18, 40, 42};
const short RGB_PINK_THRESH[3] =		{56, 17, 27};
const short RGB_PURPLE_THRESH[3] =	{26, 29, 44};
const short RGB_BROWN_THRESH[3] =		{47, 32, 20};
const short RGB_ORANGE_THRESH[3] =	{63, 25, 13};


#elif defined(LOCATION_WEEF)
const short RGB_RED_THRESH[3] = 		{58, 22, 21};
const short RGB_GREEN_THRESH[3] =		{29, 48, 23};
const short RGB_DBLUE_THRESH[3] =		{24, 33, 43};
const short RGB_YELLOW_THRESH[3] =	{44, 41, 15};
const short RGB_LBLUE_THRESH[3] =		{22, 37, 40};
const short RGB_PINK_THRESH[3] =		{49, 20, 31};
const short RGB_PURPLE_THRESH[3] =	{29, 30, 41};
const short RGB_BROWN_THRESH[3] =		{47, 30, 23};
const short RGB_ORANGE_THRESH[3] =	{56, 28, 16};

#else
const short RGB_RED_THRESH[3] = 		{0, 0, 0};
const short RGB_GREEN_THRESH[3] =		{0, 0, 0};
const short RGB_DBLUE_THRESH[3] =		{0, 0, 0};
const short RGB_YELLOW_THRESH[3] =	{0, 0, 0};
const short RGB_LBLUE_THRESH[3] =		{0, 0, 0};
const short RGB_PINK_THRESH[3] =		{0, 0, 0};
const short RGB_PURPLE_THRESH[3] =	{0, 0, 0};
const short RGB_BROWN_THRESH[3] =		{0, 0, 0};
const short RGB_ORANGE_THRESH[3] =	{0, 0, 0};

#endif

#endif
