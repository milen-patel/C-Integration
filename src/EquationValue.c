#include "EquationValue.h"
#include <math.h>
#include <ctype.h>
#include <stdio.h>

float parseNum(CharItr *itr, int *err) {
		float rVal = 0;
		if (!CharItr_has_next(itr)) {
			return rVal;
		}

		if(CharItr_peek(itr) == '(') {
			return rVal;
		} else if (CharItr_peek(itr) == ')') {
			return rVal;
		} else if (CharItr_peek(itr) == '^') {
			return rVal;
		} else if (CharItr_peek(itr) == '+') {
			return rVal;
		} else if (CharItr_peek(itr) == '-') {
			return rVal;
		}

		/* Either a digit or a decimal point */
		int nextInt = 0;
		while (CharItr_has_next(itr) && isdigit(CharItr_peek(itr))) {
			/* Insert next value into rVal */
			nextInt *= 10;
			nextInt += CharItr_next(itr) - '0';	
			continue;
		}
		rVal += nextInt;

		if (!CharItr_has_next(itr)) {
			return rVal;
		}

		/* If we make it here, we might have  decimal place */
		if (!(CharItr_peek(itr)=='.')) {
			return rVal;
		} else {
			/* Pull off the dot so we can get the number */
			CharItr_next(itr);
		}

		/* Get total decimal value */
		int decimalVal = 0;
		int times = 0;
		while(CharItr_has_next(itr) && isdigit(CharItr_peek(itr))) {
			int nextDec = CharItr_next(itr) - '0';
			decimalVal *= 10;
			decimalVal += nextDec;
			times++;
		}
	
		float decimalLiteral = decimalVal;
		while (times>0) {
			decimalLiteral /= 10;
			times--;
		}

		rVal+=decimalLiteral;
		return rVal;
}

float parseParenthesis(CharItr *itr, int *err) {
	if (!CharItr_has_next(itr)) {
		*err = 6;
		return 0;
	} else if (CharItr_peek(itr) != '(') {
		*err = 7;
		return 0;
	} else {
		/* Pull the ( */
		CharItr_next(itr);
	}
	float rVal = parseNum(itr, err);
	if (!CharItr_has_next(itr)) {
		*err = 8;
		return 0;
	} else if (CharItr_peek(itr) != ')') {
		*err = 9;
		return 0;
	} else {
		CharItr_next(itr);
		if (CharItr_has_next(itr) && CharItr_peek(itr)=='^') {
			return parseExponent(itr, rVal, err);
		}
		return rVal;
	}
}

float parseExponent(CharItr *itr, float base, int *err) {
	if (!CharItr_has_next(itr)) {
		return 0;
	} else if (CharItr_peek(itr) != '^') {
		return 0;
	} else {
		/* Pull the ^ */
		CharItr_next(itr);
		if (CharItr_peek(itr) != '(') {
			*err = 5;
			return 0;
		}
	}

	float exp = parseParenthesis(itr, err);
	//printf("Parse exp yeidling base %f with power %f\n", base, exp);
	return powf(base, exp);

}

float equationToFloat(Str equation, int *err) {
	float runningTotal = 0.0;

	/*Create a CharItr */
	CharItr itr = CharItr_of_Str(&equation);
	
	/* Use itr to read in input */
	while(CharItr_has_next(&itr)) {
		/* We want to parse one item at a time where an item is anything up to a +- */
		if (isdigit(CharItr_peek(&itr))) {
			float val = parseNum(&itr, err);
			if (*err != 0) {
				return 0;
			}
			if (!CharItr_has_next(&itr)) {
				runningTotal+=val;
				return runningTotal;
			} else if (CharItr_peek(&itr) == '-') {
				runningTotal+=val;
				continue;
			}

			/* Check if we should multiply */
			if (CharItr_peek(&itr) == '*') {
				CharItr_next(&itr); /* Pull the literal * */
				/* Parse parenthesis */
				float base = parseParenthesis(&itr,err);
				if (*err != 0) {
					return 0;
				}
				/* Check if there is an exponent */
				if (CharItr_has_next(&itr) && CharItr_peek(&itr)=='^') {
					runningTotal += (val * parseExponent(&itr, base, err));
					if (*err != 0) {
						return 0;
					}
				} else {
					runningTotal += (val * base);
				}
				continue;
			}

			/* If its addition, continue on */
			if (CharItr_peek(&itr) == '+') {
				CharItr_next(&itr); /* Pull the literal + */
				runningTotal += val;
				continue;
			}
		}
		
		/* If we have parenthesis */
		if (CharItr_peek(&itr) == '(') {
			float parenVal = parseParenthesis(&itr, err);
			if (*err != 0) {
				return 0;
			}
			/* We might still have another num */
			if (!CharItr_has_next(&itr)) {
				runningTotal += parenVal;
				return runningTotal;
			}
			/* Check for exponent */
			if (CharItr_peek(&itr) == '^') {
				runningTotal += parseExponent(&itr, parenVal, err);
				if (*err != 0) {
					return 0;
				}
			} else {
				runningTotal += parenVal;
			}
		}
		/* If we have an addition */
		if (CharItr_peek(&itr) == '+') {
			CharItr_next(&itr);
			continue;
		}	
		/* Last case is if we have a minus */
		if (CharItr_peek(&itr) == '-') {
			/* Consume the literal - */
			CharItr_next(&itr);
			/* Case 1: Minus a constant: c */
			/* Case 6: Minus a constant times something c*? */
			if (isdigit(CharItr_peek(&itr))) {
				float initialVal = parseNum(&itr, err);
				if (*err != 0) {
					return 0;
				}
				if (!CharItr_has_next(&itr)) {
					runningTotal -= initialVal;
					return runningTotal;
				}
				if (CharItr_peek(&itr) != '*') {
					if (!(CharItr_peek(&itr)=='+' || CharItr_peek(&itr)=='-')) {
						*err = 1;
						return 0;
					} else {
						runningTotal -= initialVal;
						continue;
					}
				}
					CharItr_next(&itr);
					/* Go ahead and get the next value */
					float nextNum = parseParenthesis(&itr, err);
					if (*err != 0) {
						return 0;
					}
					/* Check if nextNum has an exponent */
					if (!CharItr_has_next(&itr)) {
						runningTotal -= (initialVal*nextNum);
						continue;
					}
					if (CharItr_peek(&itr) == '^') {
						float secondProd = parseExponent(&itr, nextNum, err);
						if (*err != 0) {
							return 0;
						}
						runningTotal -= (initialVal*secondProd);
						continue;
					} else {
						runningTotal -= (initialVal*nextNum);
						/* Some basic error checking */
						if (!(CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-')) {
							*err = 2;
							return 0;
						}
						continue;
					}
				
			}
			/* Case 2: Minus a constant in parenthesis: (c) */
			/* Case 3: Minus a base^exponent: (a)^(x)*/
			/* Case 4: Minus a coeff*base^exponent: (a)*(x)^(n) */
			/* Case 5: Minus a product: (a)*(x) */
			if (CharItr_peek(&itr) == '(') {
				/* Get the value from the parenthesis */
				float initialVal = parseParenthesis(&itr, err);
				if (*err != 0) {
					return 0;
				}
				/* Check for a case 2 */
				if (!CharItr_has_next(&itr)) { runningTotal-=initialVal; return runningTotal; } /* Case 2 end of expression */
				if (CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-') {
					/* Then we have a case 2 */
					runningTotal-=initialVal;
					continue;
				}
				/* Check for case 3 */
				if (CharItr_peek(&itr) == '^') {
					runningTotal -= parseExponent(&itr, initialVal, err);
					if (*err != 0) {
						return 0;
					}
					continue;
				}
				/* Check for Cases 4 and 5 */
				if (CharItr_peek(&itr) == '*') {
					/* Pull the literal */
					CharItr_next(&itr);
					/* Go ahead and get the next value */
					float nextNum = parseParenthesis(&itr, err);
					if (*err != 0) {
						return 0;
					}
					/* Check if nextNum has an exponent */
					if (!CharItr_has_next(&itr)) {
						runningTotal -= (initialVal*nextNum);
						continue;
					}
					if (CharItr_peek(&itr) == '^') {
						float secondProd = parseExponent(&itr, nextNum, err);
						if (*err != 0) {
							return 0;
						}
						runningTotal -= (initialVal*secondProd);
						continue;
					} else {
						runningTotal -= (initialVal*nextNum);
						/* Some basic error checking */
						if (!(CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-')) {
							*err = 3;
							return 0;
						}
						continue;
					}
				}
			} else {
				*err = 4;
				return 0;
			}
		}
	}
	return runningTotal;
}
