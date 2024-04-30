#include "FlockInitialiser.h"

#include "ShaderReader.h"

#include <algorithm>

const VkPushConstantRange FlockInitialiser::pushConstantRange
{
    VK_SHADER_STAGE_COMPUTE_BIT,
    0,
    sizeof(uint32_t)
};

const VkDescriptorSetLayoutBinding FlockInitialiser::posBinding
{

    0,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    1,
    VK_SHADER_STAGE_COMPUTE_BIT,
    nullptr
};

const VkDescriptorSetLayoutBinding FlockInitialiser::velBinding
{

    1,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    1,
    VK_SHADER_STAGE_COMPUTE_BIT,
    nullptr
};

FlockInitialiser::FlockInitialiser(Device& device, uint32_t flockSize, const std::string& initShaderPath, DescriptorPool& descriptorPool)
    : flockSize(flockSize)
    , groupCount(calcGroupCount())
    , initShaderDescriptorSetLayouts(makeInitShaderDescriptorSetLayouts(device))
    , initShader(device, ShaderReader(initShaderPath).getCode(), VK_SHADER_STAGE_COMPUTE_BIT, initShaderDescriptorSetLayouts, { pushConstantRange })
    , initPipeline(device, initShader)
    , descriptorPool(descriptorPool)
    , descriptorSets()
{

}

uint32_t FlockInitialiser::calcGroupCount()
{

    return std::max(1u, flockSize / INVOCATIONS);
}

void FlockInitialiser::bindObjects(CommandBuffer& commandBuffer)
{

    initPipeline.bind(commandBuffer);

    descriptorSets[0].bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, initPipeline.getLayout());
}

std::vector<DescriptorSetInfo> FlockInitialiser::makeDescriptorSetInfos(DeviceBuffer& posBuffer, DeviceBuffer& velBuffer) const
{

    std::vector<Descriptor> descriptors;

    std::vector<const DeviceBuffer*> posBuffers{ &posBuffer };
    std::vector<const DeviceBuffer*> velBuffers{ &velBuffer };

    descriptors.emplace_back(posBinding, posBuffers);
    descriptors.emplace_back(velBinding, velBuffers);

    std::vector<DescriptorSetInfo> descriptorSetInfos;

    descriptorSetInfos.emplace_back(descriptors, &(initShaderDescriptorSetLayouts[0]));

    return descriptorSetInfos;
}

std::vector<DescriptorSetLayout> FlockInitialiser::makeInitShaderDescriptorSetLayouts(Device& device) const
{

	std::vector<DescriptorSetLayout> layouts;

	std::vector<VkDescriptorSetLayoutBinding> bindings{ posBinding, velBinding };

	layouts.emplace_back(device, bindings);

	return layouts;
}


void FlockInitialiser::initialise(CommandBuffer& commandBuffer, DeviceBuffer& posBuffer, DeviceBuffer& velBuffer)
{

    descriptorSets = descriptorPool.makeDescriptorSets(makeDescriptorSetInfos(posBuffer, velBuffer));

    bindObjects(commandBuffer);

    vkCmdPushConstants(commandBuffer.vk(), initPipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(uint32_t), &flockSize);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);
}
