/* EquationValue is responsible for converting a string expression into a literal value */

#ifndef EQNVAL_H
#define EQNVAL_H

#include "Str.h"
#include "CharItr.h"

/* Takes an equation in the format of a string and returns the
 * value that the string holds.
 *
 * In usage, this is Only capable of parsing through polynomials 
 * where each term in the polynomial can be expressed in the 
 * common form of Ax^n.
 *
 * Example Usage:
 * "5*(1.000000)^(2)+10*(1.000000)" --> 15
 * "5*(1.333333)^(2)+10*(1.333333)" --> 22.22 
 * To see more examples, run the main program and display
 * output when prompted
 *
 * Cautions:
 * Cannot take exponentials, logarithms, nested parenthesis,
 * trig functions, or any non polynomial term
 */
float equationToFloat(Str equation, int *err);

/* Takes a pointer to a CharItr and returns the numerical value 
 * of the next real number expressed in the CharItr.
 *
 * In implementation, this is a helper function for equationToFloat,
 * but could have potential values on its own.
 *
 * By assumption, the next value in itr must be a real number!
 */
float parseNum(CharItr *itr, int *err);

/* Takes a pointer to a CharItr and gets the numerical value
 * enclosed in parenthesis 
 *
 * In implementation, this is a helper function for equationToFloat,
 * but could have potential values on its own.
 *
 * By assumption, the next value in itr must be an open parenthesis!
 */
float parseParenthesis(CharItr *itr, int *err);

/* Takes a pointer to a CharItr and a base and returns the value
 * of base raised to the next exponent stored in CharItr.
 *
 * In implementation, this is a helper function for equationToFloat,
 * but could have potential values on its own.
 *
 * By assumption, itr must have a ^ as its next character
 */
float parseExponent(CharItr *itr, float base, int *err);

#endif
