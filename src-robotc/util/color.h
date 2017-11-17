#ifndef COLOR_H
#define COLOR_H


bool isInRGBPercent(const short * rgb, short red, short green, short blue, short thresh = 5);
bool isInRGBRange(const short * rgb, short minR, short minG, short minB, short maxR, short maxG, short maxB);
int getCellValue();

#endif
