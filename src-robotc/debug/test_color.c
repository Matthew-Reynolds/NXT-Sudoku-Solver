#include "test_color.h"

task main()
{

	SensorType[colorSensor] = sensorColorNxtFULL;

	while(true){
		displayCenteredBigTextLine(1, "Val: %d", getCellValue());
	}

}
