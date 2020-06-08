#include <stdio.h>
#include <stdlib.h>
#include "sds.h"

#define BAD_EQUATION_CODE 1
#define INTEGRATED_SUCCESSFULLY 0

int main() {
	/* Introduce program and prompt for equation */
	printf("Welcome to C-Integrate, this prorgram was written by Milen Patel\n");
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

	/* Free the pointers */
	free(line);
	return INTEGRATED_SUCCESSFULLY;
}
