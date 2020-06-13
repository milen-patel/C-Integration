#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Str.h"

typedef struct IntegrateRequest {
	float lowerBound;
	float upperBound;
	int numPartitions;
	Str equationStr;
} IntegrateRequest;

IntegrateRequest constructIntegrationRequest(Str equation, float lowerBound, float upperBound, int numPartitions);

float handleIntegrationRequest(IntegrateRequest *req);

typedef enum RequestValidationResult {
	VALID = 0,
	DECREASING_BOUNDS = 1,
	SAME_BOUNDS = 2,
	INVALID_PARTITION_COUNT = 3,
	ZERO_PARTITIONS = 4,
} RequestValidationResult;

RequestValidationResult validateIntegrationRequest(IntegrateRequest *req);


#endif
