#pragma once

#include "Device.h"
#include "CommandBuffer.h"
#include "DescriptorSetInfo.h"
#include "DeviceBuffer.h"
#include "DescriptorSet.h"
#include "PushConstants.h"

class FlockUpdater
{

protected:
    Device& device;

    const uint32_t queueSize;

    static uint32_t calcGroupCount(uint32_t boidCount, uint32_t localSize);

    uint32_t groupCount;

public:
    PushConstants parameters;

    FlockUpdater(
        Device& device,
        uint32_t queueSize,
        uint32_t boidCount,
        uint32_t localSize,
        PushConstants parameters
    );

    virtual void update(CommandBuffer& commandBuffer, uint32_t step) = 0;
};

