#pragma once

#include <glm/glm.hpp>

#include "CommandBuffer.h"
#include "ComputePipeline.h"
#include "DeviceBuffer.h"

#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetInfo.h"
#include "FlockInitialiser.h"
#include "FlockUpdaterNaive.h"


class Flock
{

public:

    PushConstants parameters;

private:

    static const std::unordered_set<VkDescriptorType> requiredDescriptorTypes;

    const uint32_t QUEUE_SIZE;

    uint64_t step{ 0 };

    Device& device;

    DescriptorPool descriptorPool;

    std::vector<DeviceBuffer> posBuffers;

    std::vector<DeviceBuffer> velBuffers;

    FlockInitialiser initialiser;

    FlockUpdaterNaive updater;

    void initBuffers();

protected:

    std::vector<DeviceBuffer> makeBuffers(VkDeviceSize elementSize, VkBufferUsageFlags extraUsageFlags) const;

public:

    Flock(Device& device, PushConstants parameters, uint32_t queueSize, const std::string& shaderFolder);

    void update(CommandBuffer& commandBuffer);

    DeviceBuffer& getPositionBuffer();

    uint32_t getBoidCount() const;
};

