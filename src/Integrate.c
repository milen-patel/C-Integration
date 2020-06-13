#include "Integrate.h"
#include "CharItr.h"
#include "Scanner.h"
#include "tinyexpr.h"
#include <stdio.h>

IntegrateRequest constructIntegrationRequest(Str equation, float lowerBound, float upperBound, int numPartitions) {
	IntegrateRequest rval = {
		lowerBound,
		upperBound,
		numPartitions,
		equation
	};
	return rval;
}

float handleIntegrationRequest(IntegrateRequest *req) {
	/* Let the user know what we are doing */
	printf("The Following Request Has Been Made to Integrate from %f to %f\n", req->lowerBound, req->upperBound);

	/* Verify Input */
	if (validateIntegrationRequest(req) == ZERO_PARTITIONS) {
		return 0.00;
	} else if (validateIntegrationRequest(req) == SAME_BOUNDS) {
		return 0.00;
	} else if (validateIntegrationRequest(req) != VALID) {
		/* TODO: Handle bad input */
		return 0.00; 
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
		float xVal = i*boxWidth;
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
					/* Want to convert a float to string before subsituting */
					char strval[20];
					sprintf(strval, "%f", xVal);
					Str_append(&numericEquation, "*(");
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
		printf("The equation is: %s\n", Str_cstr(&numericEquation));
		printf("----> %f\n", te_interp(Str_cstr(&numericEquation), 0));
		integrationTotal += boxWidth*te_interp(Str_cstr(&numericEquation), 0);
		Str_drop(&numericEquation);
	}
	printf("TOTAL: %f\n", integrationTotal);
	return 0.00;
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
	return VALID;
}
