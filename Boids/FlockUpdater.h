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
    const Device& device;

    const uint32_t queueSize;

    static uint32_t calcGroupCount(uint32_t boidCount, uint32_t localSize);

    uint32_t groupCount;

    virtual std::vector<DescriptorSetInfo> makeDescriptorSetInfos(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const = 0;

    virtual void bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const = 0;

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

