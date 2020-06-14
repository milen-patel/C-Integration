#ifndef EQNVAL_H
#define EQNVAL_H

#include "Str.h"
#include "CharItr.h"

float parseNum(CharItr *itr, int *err);
float parseParenthesis(CharItr *itr, int *err);
float parseExponent(CharItr *itr, float base, int *err);
float equationToFloat(Str equation, int *err);


#endif
