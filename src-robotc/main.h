// We get some funky business going on here.
// Since we have no linker, sources are included rather than headers.
// Each source has its own header where it and its dependencies are defined.
// This is a very irregular organization of code but is neccesary due to
// the lack of a linker in RobotC
#ifndef MATH_H
#include "util/math.c"
#endif

#ifndef PI_CONTROLLER_H
#include "util/pi_controller.c"
#endif
