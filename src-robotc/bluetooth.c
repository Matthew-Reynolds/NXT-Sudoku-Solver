#include "bluetooth.h"


char * getErrorMessage(BT_Status error){
	string errorMessage = "";

	if(error == BT_ERROR)
			errorMessage = "BT Error!";
	else if (error == BT_TIMEOUT)
			errorMessage = "BT Timeout!";

		return errorMessage;
}
