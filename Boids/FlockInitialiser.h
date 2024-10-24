#pragma once

#include "vulkan/vulkan.h"

#include "Device.h"
#include "DeviceBuffer.h"
#include "CommandBuffer.h"
#include "ComputePipeline.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSetPack.h"
#include "DescriptorPool.h"
#include "BoidInitShader.h"

class FlockInitialiser
{

    static constexpr uint32_t LOCAL_SIZE{ 128 };

    const uint32_t flockSize, groupCount;

    Device& device;

    BoidInitShader shader;

    ComputePipeline initPipeline;

    DescriptorPool& descriptorPool;

    DescriptorSetPack descriptorSets;

    uint32_t calcGroupCount();

    void bindObjects(CommandBuffer& commandBuffer);

public:
    FlockInitialiser(Device& device, uint32_t flockSize, const std::string& shaderFolder, DescriptorPool& descriptorPool);

    void initialise(CommandBuffer& commandBuffer, DeviceBuffer& posBuffer, DeviceBuffer& velBuffer);
};

