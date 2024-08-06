#pragma once

#include "FlockUpdater.h"
#include "Shader.h";
#include "ComputePipeline.h"
#include "DescriptorPool.h"
#include "PushConstants.h"

class FlockUpdaterSweepAndPrune : public FlockUpdater
{
    static constexpr uint32_t LOCAL_SIZE{ 128 };

    static const VkDescriptorSetLayoutBinding inPosBinding, inVelBinding, outPosBinding, outVelBinding;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    Shader shader;

    ComputePipeline pipeline;

    std::vector<DescriptorSet> descriptorSets;

    virtual std::vector<DescriptorSetInfo> makeDescriptorSetInfos(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const;

    virtual void bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const;

public:

    PushConstants parameters;

    FlockUpdaterSweepAndPrune(
        Device& device,
        const std::string& computeShaderPath,
        DescriptorPool& descriptorPool,
        uint32_t boidCount,
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers,
        PushConstants parameters
    );

    virtual void update(CommandBuffer& commandBuffer, uint32_t step);
};

