#ifndef COLOR_H
#define COLOR_H

#include "main.h"

void getAvgColor(short * rgb, int readings = 5, long interval = 10);
bool isInRGBPercent(const short * rgb, const short * percents, short thresh = 5);
bool isInRGBPercent(const short * rgb, short red, short green, short blue, short thresh = 5);
bool isInRGBRange(const short * rgb, short minR, short minG, short minB, short maxR, short maxG, short maxB);
int getCellValue();


// LOAD THE CONSTANTS FOR THE RGB THRESHOLDS
//#define LOCATION_SJU
//#define LOCATION_WATIMAKE
#define LOCATION_WEEF

#ifdef LOCATION_SJU
const short RGB_RED_THRESH[3] = 		{64, 18, 16};
const short RGB_GREEN_THRESH[3] =		{27, 47, 25};
const short RGB_DBLUE_THRESH[3] =		{24, 34, 43};
const short RGB_YELLOW_THRESH[3] =	{44, 39, 16};
const short RGB_LBLUE_THRESH[3] =		{21, 38, 39};
const short RGB_PINK_THRESH[3] =		{58, 17, 24};
const short RGB_PURPLE_THRESH[3] =	{42, 24, 33};
const short RGB_BROWN_THRESH[3] =		{48, 29, 21};
const short RGB_ORANGE_THRESH[3] =	{58, 24, 16};
const short RGB_WHITE_THRESH[3] =		{33, 33, 33};

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
const short RGB_WHITE_THRESH[3] =		{33, 33, 33};


#elif defined(LOCATION_WEEF)
const short RGB_RED_THRESH[3] = 		{59, 21, 18};
const short RGB_GREEN_THRESH[3] =		{28, 45, 26};
const short RGB_DBLUE_THRESH[3] =		{25, 32, 43};
const short RGB_YELLOW_THRESH[3] =	{44, 38, 17};
const short RGB_LBLUE_THRESH[3] =		{22, 38, 39};
const short RGB_PINK_THRESH[3] =		{54, 19, 26};
const short RGB_PURPLE_THRESH[3] =	{41, 25, 32};
const short RGB_BROWN_THRESH[3] =		{48, 30, 21};
const short RGB_ORANGE_THRESH[3] =	{54, 27, 17};
const short RGB_WHITE_THRESH[3] =		{33, 34, 31};

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
const short RGB_WHITE_THRESH[3] =		{0, 0, 0};

#endif

#endif
