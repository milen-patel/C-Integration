#include <stdio.h>
#include <stdlib.h>
#include "sds.h"
#include "Scanner.h"
#include "CharItr.h"
#include "Str.h"
#include "Vec.h"

#define BAD_EQUATION_CODE 1
#define INTEGRATED_SUCCESSFULLY 0

int main() {
	/* Introduce program and prompt for equation */
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
	//Scanner scan = Scanner_value(CharItr_of_Str(&lineString));
	//while(Scanner_has_next(&scan)) {
		//Token next = Scanner_next(&scan);
		//printf(">%s\n", Str_cstr(&next.lexeme));
		//Str_drop(&next.lexeme);
	//}

	Str_drop(&lineString);
	/* Free the pointers */
	free(line);

	return INTEGRATED_SUCCESSFULLY;
}
