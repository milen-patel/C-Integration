#include "Integrate.h"
#include "CharItr.h"
#include "Scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>

float parseNum(CharItr *itr);
float equationToFloat(Str equation);
float parseExponent(CharItr *itr, float base);

IntegrateRequest constructIntegrationRequest(Str equation, float lowerBound, float upperBound, int numPartitions, bool showOutput) {
	IntegrateRequest rval = {
		lowerBound,
		upperBound,
		numPartitions,
		equation,
		showOutput,
	};
	return rval;
}

void handleIntegrationRequest(IntegrateRequest *req) {
	/* Let the user know what we are doing */
	printf("The Following Request Has Been Made to Integrate from %f to %f\n", req->lowerBound, req->upperBound);

	/* Verify Input */
	if (validateIntegrationRequest(req) == ZERO_PARTITIONS) {
		printf("TOTAL: 0\n");
		return; 
	} else if (validateIntegrationRequest(req) == SAME_BOUNDS) {
		printf("TOTAL: 0\n");
		return; 
	} else if (validateIntegrationRequest(req) != VALID) {
		printf("Unable to integrate due to invalid input!\n");
		return; 
	} else if (validateIntegrationRequest(req) == NEGATIVE_BOUNDS) {
		printf("Unable to integrate with negative bounds!\n");
		return;
	}

	/* For large partition values, notify the user */
	if (req->numPartitions > 1500) {
		printf("Warning: The program may run slower due to a large partition count\n");
	}

	/* Calculate box width */
	float boxWidth = (req->upperBound - req->lowerBound)/(req->numPartitions);
	float integrationTotal = 0;

	/* Start calculating and adding areas */
	for (int i=0; i<req->numPartitions; i++) {
		/* Construct a iterator for the equation Str */
		CharItr itr = CharItr_of_Str(&req->equationStr);
		Scanner scanner = Scanner_value(itr);

		/* Create String to hold equation with subsituted values */
		Str numericEquation = Str_value(1);
		float xVal = i*boxWidth + req->lowerBound;

		/* Now, calculate the area of the current rectangle by parsing the equaton */
		while(Scanner_has_next(&scanner)) {
			Token nextToken = Scanner_next(&scanner);
			switch (nextToken.type) {
				case END_TOKEN:
					break;
				case NUMERIC_TOKEN:
					Str_append(&numericEquation, Str_cstr(&nextToken.lexeme));
					break;
				case ADD_TOKEN:
					Str_append(&numericEquation, "+");
					break;
				case SUBTRACT_TOKEN:
					Str_append(&numericEquation, "-");
					break;
				case VARIABLE_TOKEN:
					{		
						/* If the previous term is a coefficient, add a * */
						if (Str_length(&numericEquation)>0 && isdigit(*Str_ref(&numericEquation, Str_length(&numericEquation)-1))) {
							Str_append(&numericEquation, "*");
						}

						/* Want to convert a float to string before subsituting */
						char strval[20];
						sprintf(strval, "%f", xVal);
						Str_append(&numericEquation, "(");
						Str_append(&numericEquation, strval);
						Str_append(&numericEquation, ")");
						break;
					}
				case EXPONENT_TOKEN:
					{
						Str_append(&numericEquation, "^(");
						Str_append(&numericEquation, Str_cstr(&nextToken.lexeme));
						Str_append(&numericEquation, ")");
						break;
					}
				case WORD_TOKEN:
					break;
				default:
					break;
			}
			Str_drop(&nextToken.lexeme);
		}

		/* If the user asks to show output, then display it after each rectangle */
		if (req->showOutput) {
			printf("The equation of the current box is: %s\n", Str_cstr(&numericEquation));
			printf("= %f\n", equationToFloat(numericEquation));
		}

		/* Add the current rectangle to the total integral value and drop the string */
		integrationTotal += boxWidth*equationToFloat(numericEquation);
		Str_drop(&numericEquation);
	}
	printf("TOTAL: %f\n", integrationTotal);
}

RequestValidationResult validateIntegrationRequest(IntegrateRequest *req) {
	if (req->lowerBound > req->upperBound) {
		return DECREASING_BOUNDS;
	}
	if (req->lowerBound == req->upperBound) {
		return SAME_BOUNDS;	
	}
	if (req->numPartitions == 0) {
		return ZERO_PARTITIONS;
	}
	if (req->numPartitions < 0) {
		return INVALID_PARTITION_COUNT;
	}
	if (req->lowerBound < 0 || req->upperBound < 0) {
		return NEGATIVE_BOUNDS;
	}

	/* Read the equation itself */

	return VALID;
}

float parseNum(CharItr *itr) {
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

float parseParenthesis(CharItr *itr) {
	if (!CharItr_has_next(itr)) {
		printf("(0)ERROR\n");
		return 0;
	} else if (CharItr_peek(itr) != '(') {
		printf("(1)ERROR\n");
		return 0;
	} else {
		/* Pull the ( */
		CharItr_next(itr);
	}
	float rVal = parseNum(itr);
	if (!CharItr_has_next(itr)) {
		printf("(2)ERROR\n");
		return 0;
	} else if (CharItr_peek(itr) != ')') {
		printf("(3)ERROR\n");
		return 0;
	} else {
		CharItr_next(itr);
		if (CharItr_has_next(itr) && CharItr_peek(itr)=='^') {
			return parseExponent(itr, rVal);
		}
		return rVal;
	}
}

float parseExponent(CharItr *itr, float base) {
	if (!CharItr_has_next(itr)) {
		return 0;
	} else if (CharItr_peek(itr) != '^') {
		return 0;
	} else {
		/* Pull the ^ */
		CharItr_next(itr);
		if (CharItr_peek(itr) != '(') {
			printf("(4)ERROR\n");
			return 0;
		}
	}

	float exp = parseParenthesis(itr);
	//printf("Parse exp yeidling base %f with power %f\n", base, exp);
	return powf(base, exp);

}

float equationToFloat(Str equation) {
	float runningTotal = 0.0;

	/*Create a CharItr */
	CharItr itr = CharItr_of_Str(&equation);
	
	/* Use itr to read in input */
	while(CharItr_has_next(&itr)) {
		/* We want to parse one item at a time where an item is anything up to a +- */
		if (isdigit(CharItr_peek(&itr))) {
			float val = parseNum(&itr);
			if (!CharItr_has_next(&itr)) {
				runningTotal+=val;
				//printf("(0) Adding %f to %f\n",val,runningTotal);
				return runningTotal;
			} else if (CharItr_peek(&itr) == '-') {
				runningTotal+=val;
				//printf("(0.5) Adding %f to %f\n",val,runningTotal);
				continue;
			}

			/* Check if we should multiply */
			if (CharItr_peek(&itr) == '*') {
				CharItr_next(&itr); /* Pull the literal * */
				//runningTotal += (val * parseParenthesis(&itr));
				/* Parse parenthesis */
				float base = parseParenthesis(&itr);
				/* Check if there is an exponent */
				if (CharItr_has_next(&itr) && CharItr_peek(&itr)=='^') {
					runningTotal += (val * parseExponent(&itr, base));
				} else {
					runningTotal += (val * base);
				}
				//printf("(1) Total: %f\n",runningTotal);
				continue;
			}

			/* If its addition, continue on */
			if (CharItr_peek(&itr) == '+') {
				CharItr_next(&itr); /* Pull the literal + */
				runningTotal += val;
				//printf("(2) Adding %f to %f\n",val,runningTotal);
				continue;
			}
		}
		
		/* If we have parenthesis */
		if (CharItr_peek(&itr) == '(') {
			float parenVal = parseParenthesis(&itr);
			/* We might still have another num */
			if (!CharItr_has_next(&itr)) {
				runningTotal += parenVal;
				return runningTotal;
			}
			/* Check for exponent */
			if (CharItr_peek(&itr) == '^') {
				runningTotal += parseExponent(&itr, parenVal);
				//printf("(3) Total:  %f\n",runningTotal);
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
				float initialVal = parseNum(&itr);
				if (!CharItr_has_next(&itr)) {
					runningTotal -= initialVal;
					//printf("(19) Subtracted c, now have %f\n", runningTotal);
					return runningTotal;
				}
				if (CharItr_peek(&itr) != '*') {
					if (!(CharItr_peek(&itr)=='+' || CharItr_peek(&itr)=='-')) {
						printf("(66) ERROR\n");
					} else {
						runningTotal -= initialVal;
						//printf("(-19) Subtracted c, now have %f\n", runningTotal);
						continue;
					}
				}
					CharItr_next(&itr);
					/* Go ahead and get the next value */
					float nextNum = parseParenthesis(&itr);
					/* Check if nextNum has an exponent */
					if (!CharItr_has_next(&itr)) {
						runningTotal -= (initialVal*nextNum);
						//printf("(28) Subtracted now at %f\n", runningTotal);
						continue;
					}
					if (CharItr_peek(&itr) == '^') {
						float secondProd = parseExponent(&itr, nextNum);
						runningTotal -= (initialVal*secondProd);
						//printf("(29) Subtracted now at %f\n", runningTotal);
						continue;
					} else {
						runningTotal -= (initialVal*nextNum);
						//printf("(30) Subtracted now at %f\n", runningTotal);
						/* Some basic error checking */
						if (!(CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-')) {
							printf("(31) Error\n");
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
				float initialVal = parseParenthesis(&itr);
				/* Check for a case 2 */
				if (!CharItr_has_next(&itr)) { runningTotal-=initialVal; return runningTotal; } /* Case 2 end of expression */
				if (CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-') {
					/* Then we have a case 2 */
					runningTotal-=initialVal;
					//printf("(20) Subtracting %f with new total %f\n", initialVal, runningTotal);
					continue;
				}
				/* Check for case 3 */
				if (CharItr_peek(&itr) == '^') {
					runningTotal -= parseExponent(&itr, initialVal);
					//printf("(21) Subtracted exp prod now at %f\n", runningTotal);
					continue;
				}
				/* Check for Cases 4 and 5 */
				if (CharItr_peek(&itr) == '*') {
					/* Pull the literal */
					CharItr_next(&itr);
					/* Go ahead and get the next value */
					float nextNum = parseParenthesis(&itr);
					/* Check if nextNum has an exponent */
					if (!CharItr_has_next(&itr)) {
						runningTotal -= (initialVal*nextNum);
						//printf("(22) Subtracted now at %f\n", runningTotal);
						continue;
					}
					if (CharItr_peek(&itr) == '^') {
						float secondProd = parseExponent(&itr, nextNum);
						runningTotal -= (initialVal*secondProd);
						//printf("(23) Subtracted now at %f\n", runningTotal);
						continue;
					} else {
						runningTotal -= (initialVal*nextNum);
						//printf("(24) Subtracted now at %f\n", runningTotal);
						/* Some basic error checking */
						if (!(CharItr_peek(&itr) == '+' || CharItr_peek(&itr) == '-')) {
							printf("(11) Error\n");
						}
						continue;
					}
				}
			} else {
				printf("(55) Error: Unexpected: %c", CharItr_peek(&itr));
			}
		}
	}
	return runningTotal;
}
