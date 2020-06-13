#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Scanner.h"

typedef struct IntegrateRequest {
	int lowerBound;
	int upperBound;
	int numPartitions;
	Scanner equationScanner;
} IntegrateRequest;

IntegrateRequest constructIntegrationRequest(Scanner equation, int lowerBound, int upperBound, int numPartitions);

void handleIntegrationRequest(IntegrateRequest *req);

typedef enum RequestValidationResult {
	VALID = 0,
	DECREASING_BOUNDS = 1,
	SAME_BOUNDS = 2,
	INVALID_PARTITION_COUNT = 3,
	ZERO_PARTITIONS = 4,
} RequestValidationResult;

RequestValidationResult validateIntegrationRequest(IntegrateRequest *req);


#endif
