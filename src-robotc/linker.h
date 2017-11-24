// We've get some funky business going on here.
// Since we have no linker, sources are included rather than headers.
// Each source has its own header where it and its dependencies are defined.
// This is a very irregular organization of code but is neccesary due to
// the lack of a linker in RobotC
#ifndef LINKER_H
#define LINKER_H
#include "util/math.c"
#include "util/pi_controller.c"
#include "util/color.c"
#include "movement.c"
#include "bluetooth.c"
#endif
