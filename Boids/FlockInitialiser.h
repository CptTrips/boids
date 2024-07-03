#pragma once

#include "vulkan/vulkan.h"

#include "Device.h"
#include "DeviceBuffer.h"
#include "CommandBuffer.h"
#include "ComputePipeline.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"

class FlockInitialiser
{

    static constexpr uint32_t LOCAL_SIZE{ 128 };

    static const VkPushConstantRange pushConstantRange;

    static const VkDescriptorSetLayoutBinding posBinding, velBinding;

    const uint32_t flockSize, groupCount;

    std::vector<DescriptorSetLayout> initShaderDescriptorSetLayouts;

    Shader initShader;

    ComputePipeline initPipeline;

    DescriptorPool& descriptorPool;

    std::vector<DescriptorSet> descriptorSets;

    uint32_t calcGroupCount();

    void bindObjects(CommandBuffer& commandBuffer);

    std::vector<DescriptorSetInfo> makeDescriptorSetInfos(DeviceBuffer& posBuffer, DeviceBuffer& velBuffer) const;

    std::vector<DescriptorSetLayout> makeInitShaderDescriptorSetLayouts(Device& device) const;

public:
    FlockInitialiser(Device& device, uint32_t flockSize, const std::string& initShaderPath, DescriptorPool& descriptorPool);

    void initialise(CommandBuffer& commandBuffer, DeviceBuffer& posBuffer, DeviceBuffer& velBuffer);
};

