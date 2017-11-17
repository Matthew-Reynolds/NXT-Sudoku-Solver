# NXT Sudoku Solver
Final design project for 1A Mechatronics Engineering at UWaterloo. A robot designed to read and solve a sudoku puzzle on paper.

# Description
This system uses two NXT bricks to solve the puzzle. One brick operates a pen and color sensor on 2.5 axis to read in the puzzle and write out the puzzle. Due to the memory limitations of the NXT, we use a second brick to do the actual solving of the puzzle. The main brick reads the puzzle and sends it over Bluetooth so the solver brick, which will compute the solution (if it exists) and return the solution to the main brick. From here, the main brick simply writes out the solution on the paper using its pen.

Designed using the Lego Mindstorms NXT platform. Programmed in RobotC.

# Authors:
Matthew Reynolds
Dawson Hemphill
Andrew Drury
