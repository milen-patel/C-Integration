# C-Integration
This is a program created in C that can evaluate definite integrals of polynomials

## Project Explanation
This program uses numeric integration to evaluate definite integrals; the program will divide the target region into smaller rectangles and computes the integral by summing the values of the areas of each of the smaller rectangles.\

The user is able to control the accuracy of the integral by choosing how many rectangles the input region should be sub-divided into. Fewer subdivisions will decrease the runtime of the program but will result in far less accurate integrals. Conversely, choosing high counts of subdivisions will be much closer to the true values of the integrals, but will take far more time to compute.

## Install
This project requires no dependencies. To get started, simply compile all of the sources files together!\

Alternatively, a Makefile has been provided. Use 'make all' to compile the source code and 'make run' to test it out.\

If you have any issues, then 'make clean' will remove all build files.

# Future Goals
This implementation is by no means the most efficient means of numerical integration. That being said, there are some steps that I would like to take in the future to increase the efficiency of this program. 
* Add support for more equations: logs, exponentials, trig functions, etc.
* Make the EquationValue use a recursive descent parser to improve readability of the implementation
* Small things: Update makefile, more validation, more error handling
