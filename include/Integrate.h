#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Str.h"

/* Contains the information that main will send to an integeration handler */
typedef struct IntegrateRequest {
	float lowerBound;
	float upperBound;
	int numPartitions;
	Str equationStr;
	bool showOutput;
} IntegrateRequest;


typedef enum RequestValidationResult {
	VALID = 0,
	DECREASING_BOUNDS = 1,
	SAME_BOUNDS = 2,
	INVALID_PARTITION_COUNT = 3,
	ZERO_PARTITIONS = 4,
	NEGATIVE_BOUNDS = 5,
	BAD_EQUATION = 6,
} RequestValidationResult;

/* Validates an integration request constructed by the user,
 * currently tests for negative bounds, decreasing bounds,
 * equation validitity, same bounds, and invalid partition counts
 */
RequestValidationResult validateIntegrationRequest(IntegrateRequest *req);

/* Constructs an IntegrateRequest struct */
IntegrateRequest constructIntegrationRequest(Str equation, float lowerBound, float upperBound, int numPartitions, bool showOutput);

/* Consumes an IntegrateReqest and computes the value while
 * progress to user
 */
void handleIntegrationRequest(IntegrateRequest *req);


#endif
