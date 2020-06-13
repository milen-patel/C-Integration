#include "Integrate.h"
#include <stdio.h>

IntegrateRequest constructIntegrationRequest(Scanner equation, int lowerBound, int upperBound) {
	IntegrateRequest rval = {
		lowerBound,
		upperBound,
		equation
	};
	return rval;
}

void handleIntegrationRequest(IntegrateRequest *req) {
	printf("The Following Request Has Been Made to Integrate from %d to %d\n", req->lowerBound, req->upperBound);
	Scanner scanner = req->equationScanner;
    while (Scanner_has_next(&scanner)) {
        Token next = Scanner_next(&scanner);
        switch (next.type) {
            case END_TOKEN: 
                printf("END\n");
                break;
            case WORD_TOKEN:
                printf("WORD(%s)\n", Str_cstr(&next.lexeme));
                break;
            case PIPE_TOKEN:
                printf("PIPE\n");
                break;
			case ADD_TOKEN:
				printf("ADDITION\n");
				break;
			case SUBTRACT_TOKEN:
				printf("SUBTRACTION\n");
				break;
			case VARIABLE_TOKEN:
				printf("VARIABLE(%s)\n", Str_cstr(&next.lexeme));
				break;
			case EXPONENT_TOKEN:
				printf("EXPONENT(%s)\n", Str_cstr(&next.lexeme));
				break;
			case NUMERIC_TOKEN:
				printf("NUMBER(%s)\n", Str_cstr(&next.lexeme));
				break;
        }
        Str_drop(&next.lexeme);
    }
}
