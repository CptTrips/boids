#pragma once

#include <string>

#include "FlockUpdater.h"
#include "DescriptorSetLayout.h"
#include "Shader.h";
#include "ComputePipeline.h"
#include "DescriptorPool.h"

struct PushConstants
{

    uint32_t boidCount;
    float cohesion, alignment, separation;
};

class FlockUpdaterNaive :
    public FlockUpdater
{

private:

    static constexpr uint32_t LOCAL_SIZE{ 128 };

    static const VkDescriptorSetLayoutBinding inPosBinding, inVelBinding, outPosBinding, outVelBinding;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const VkPushConstantRange pushConstantRange;

    const Device& device;

    const uint32_t queueSize;

    Shader shader;

    uint32_t groupCount;

    ComputePipeline pipeline;

    std::vector<DescriptorSet> descriptorSets;

    uint32_t calcGroupCount(uint32_t boidCount);

    std::vector<DescriptorSetInfo> makeDescriptorSetInfos(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const;

    void bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const;

public:

    PushConstants parameters;

    FlockUpdaterNaive(
        Device& device,
        const std::string& computeShaderPath,
        DescriptorPool& descriptorPool,
        uint32_t boidCount,
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers,
        PushConstants parameters
    );

    void update(CommandBuffer& commandBuffer, uint32_t step);
};
