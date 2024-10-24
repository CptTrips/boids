#pragma once

#include <vector>

#include "Device.h"
#include "CommandBuffer.h"
#include "DescriptorSetInfo.h"
#include "DeviceBuffer.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "PushConstants.h"
#include "Shader.h"
#include "ComputePipeline.h"
#include "DescriptorSetPack.h"

class ComputeWork
{

    const Device& device;

    const uint32_t queueSize;

    const uint32_t localSize;

    uint32_t groupCount;

    DescriptorSetPack descriptorSets;

    ComputePipeline pipeline;

    void bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const;

public:

    PushConstants parameters;

    ComputeWork(
        Device& device,
        DescriptorPool& descriptorPool,
        uint32_t queueSize,
        uint32_t groupCount,
        uint32_t localSize,
        PushConstants parameters,
        const Shader& shader,
        std::vector<std::vector<const DeviceBuffer*>> buffers
    );

    void dispatch(CommandBuffer& commandBuffer, uint32_t step);

};

