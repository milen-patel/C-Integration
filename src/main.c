#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Str.h"
#include "Scanner.h"
#include "Integrate.h"


#define BAD_EQUATION_CODE 1
#define BAD_INTEGRATION_BOUNDS 2
#define INTEGRATED_SUCCESSFULLY 0

void handle(Scanner scanner, int first, int second);

int main()
{
	printf("Welcome to C-Integrate, this program was written by Milen Patel\n");
	printf("Please enter a polynomial on the next line\n");
	printf("Equation:");

	char *line = NULL;
	size_t size;

	/* Use getline() to read in equation */
	if (getline(&line, &size, stdin) == -1) {
		printf("ERROR: No equation was entered!");
		return BAD_EQUATION_CODE;
	} else {
		printf("Parsing the Equation: %s", line);
	}
	line[size-1] = '\0';
	Str lineString = Str_from(line);


	/* Prompt User for bounds */
	int boundLow, boundHigh;
	printf("Enter Lower Bound: ");
	if (scanf("%d", &boundLow) != 1) {
		/* We failed to read in an integer */
		Str_drop(&lineString);
		free(line);
		printf("ABORT: Bad Lower Integration Bounds\n");
		return BAD_INTEGRATION_BOUNDS;
	}
	printf("Enter Upper Bound: ");
	if (scanf("%d", &boundHigh) != 1) {
		/* We failed to read in an integer */
		Str_drop(&lineString);
		free(line);
		printf("ABORT: Bad Upper Integration Bounds\n");
		return BAD_INTEGRATION_BOUNDS;
	}
	
	Scanner eqnScanner = Scanner_value(CharItr_of_Str((&lineString)));
	handle(eqnScanner, boundLow, boundHigh);

	/* Free the pointers and drop the string*/
	free(line);
	Str_drop(&lineString);

    return INTEGRATED_SUCCESSFULLY;
}

void handle(Scanner scanner, int first, int second) {
	/* Construct integration request */
	IntegrateRequest currentRequest = constructIntegrationRequest(scanner, first, second);
	handleIntegrationRequest(&currentRequest);
}
