#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Scanner.h"

typedef struct IntegrateRequest {
	int lowerBound;
	int upperBound;
	Scanner equationScanner;
} IntegrateRequest;

IntegrateRequest constructIntegrationRequest(Scanner equation, int lowerBound, int upperBound);

void handleIntegrationRequest(IntegrateRequest *req);


#endif
