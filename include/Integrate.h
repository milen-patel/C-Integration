#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Str.h"

typedef struct IntegrateRequest {
	float lowerBound;
	float upperBound;
	int numPartitions;
	Str equationStr;
	bool showOutput;
} IntegrateRequest;

IntegrateRequest constructIntegrationRequest(Str equation, float lowerBound, float upperBound, int numPartitions, bool showOutput);

void handleIntegrationRequest(IntegrateRequest *req);

typedef enum RequestValidationResult {
	VALID = 0,
	DECREASING_BOUNDS = 1,
	SAME_BOUNDS = 2,
	INVALID_PARTITION_COUNT = 3,
	ZERO_PARTITIONS = 4,
	NEGATIVE_BOUNDS = 5,
} RequestValidationResult;

RequestValidationResult validateIntegrationRequest(IntegrateRequest *req);


#endif
