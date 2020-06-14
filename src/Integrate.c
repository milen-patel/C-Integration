#include "Integrate.h"
#include "CharItr.h"
#include "Scanner.h"
#include "EquationValue.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>

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

		int errHandler = 0;	
		float equationValue = equationToFloat(numericEquation, &errHandler);

		/* Check for error */
		if (errHandler != 0) {
			printf("This program cannot parse equations of this type\n");
			return;
		}

		/* If the user asks to show output, then display it after each rectangle */
		if (req->showOutput) {
			printf("The equation of the current box is: %s\n", Str_cstr(&numericEquation));
			printf("= %f\n", equationValue);
		}

		/* Add the current rectangle to the total integral value and drop the string */
		integrationTotal += boxWidth*equationValue;
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

