#pragma once

#include <string>

#include "FlockUpdater.h"
#include "Shader.h";
#include "ComputePipeline.h"
#include "DescriptorPool.h"
#include "PushConstants.h"
#include "DescriptorSetPack.h"
#include "BoidUpdateNaiveShader.h"

class FlockUpdaterNaive :
    public FlockUpdater
{

    static constexpr uint32_t LOCAL_SIZE{ 128 };

    BoidUpdateNaiveShader shader;

    ComputePipeline pipeline;

    DescriptorSetPack descriptorSets;

    std::vector<std::vector<const DeviceBuffer*>> makeBufferPointers(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const;

    void bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const;

public:

    FlockUpdaterNaive(
        Device& device,
        const std::string& shaderFolder,
        DescriptorPool& descriptorPool,
        uint32_t boidCount,
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers,
        PushConstants parameters
    );

    virtual void update(CommandBuffer& commandBuffer, uint32_t step);
};
