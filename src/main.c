#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Str.h"
#include "Scanner.h"
#include "Integrate.h"

#define BAD_EQUATION_CODE 1
#define BAD_INTEGRATION_BOUNDS 2
#define BAD_PARTITION_COUNT 3
#define INTEGRATED_SUCCESSFULLY 0

void handle(Str equation, float first, float second, int n, bool showOuput);

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
	}
	
	line[size-1] = '\0';
	Str lineString = Str_from(line);


	/* Prompt User for bounds */
	float boundLow, boundHigh;
	printf("Enter Lower Bound: ");
	if (scanf("%f", &boundLow) != 1) {
		/* We failed to read in an integer */
		Str_drop(&lineString);
		free(line);
		printf("ABORT: Bad Lower Integration Bounds\n");
		return BAD_INTEGRATION_BOUNDS;
	}
	printf("Enter Upper Bound: ");
	if (scanf("%f", &boundHigh) != 1) {
		/* We failed to read in an integer */
		Str_drop(&lineString);
		free(line);
		printf("ABORT: Bad Upper Integration Bounds\n");
		return BAD_INTEGRATION_BOUNDS;
	}
	
	/* Prompt User for number of rectangles to use */
	int n;
	printf("Enter Number of Partitions: ");
	if (scanf("%d", &n) != 1) {
		/* We failed to read in an integer */
		Str_drop(&lineString);
		free(line);
		printf("ABORT: Unable to read number of partitions\n");
		return BAD_PARTITION_COUNT;
	}

	/* Prompt user for output logging */
	char showOutput;
	bool showOutputBool;
	printf("Show Output? (y/n): ");
	if (scanf(" %c", &showOutput) != 1) {
		/* Don't terminate the program, just dont show output */
		showOutputBool=false;
	}
	if (showOutput == 'y') {
		showOutputBool = true;
	} else {
		showOutputBool = false;
	}

	/* Compute integral */
	handle(lineString, boundLow, boundHigh, n, showOutputBool);
	
	/* Free the pointers and drop the string*/
	free(line);
	Str_drop(&lineString);

    return INTEGRATED_SUCCESSFULLY;
}

void handle(Str eqn, float first, float second, int n, bool showOutput) {
	/* Construct integration request */
	IntegrateRequest currentRequest = constructIntegrationRequest(eqn, first, second, n, showOutput);
	handleIntegrationRequest(&currentRequest);
}
