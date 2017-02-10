# Cube_Sim

This program simulates a Rubik's cube in two dimensions by rolling it out into a t-shape.

##Features
* Color for each of the faces to easily read the cube while scrambled.
* The ability to turn slices of the cube in addition to faces (i.e. Turning the middle layer of a 3x3x3)
* Moves can be clockwise or counter-clockwise.
* The ability to change the size of the cube you're simulating.
* A log of your most recent moves appears to the right of the cube.

##Installation
1. Install any of the requirements that you do not have.
2. Clone the project: `git clone https://github.com/Lankly/Cube_Sim.git`
3. Change to the new directory: `cd Cube_Sim/`
4. Run `cmake .` then `make`

##Controls
* Each face is specified with the first letter of its name:

  Upper (Top) = "U"  
  Down (Bottom) = "D"  
  Left = "L"  
  Right = "R"  
  Front = "F"  
  Back = "B"  
* To turn a slice, such as the middle layer on a 3x3, just specify the depth before the face's letter, like this: "2U"
* To turn counter-clockwise, simply add an apostrophe to the end of the line, like this: "2U'"
* Pressing enter with no move specified will repeat the most used move.
* 'q' quits the program.
* '?' brings you to this page.
* 'n' creates a new cube and allows you to set the size.

##Requirements
1. cmake
2. make
3. A C compiler
4. ncurses

