#include "FlockUpdater.h"
#include <algorithm>

uint32_t FlockUpdater::calcGroupCount(uint32_t boidCount, uint32_t localSize)
{

    return std::max(1u, (boidCount + localSize - 1) / localSize);
}

FlockUpdater::FlockUpdater(Device& device, uint32_t queueSize, uint32_t boidCount, uint32_t localSize, PushConstants parameters)
	: device(device)
	, queueSize(queueSize)
	, groupCount(calcGroupCount(boidCount, localSize))
	, parameters(parameters)
{
}
