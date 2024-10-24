#include "FlockInitialiser.h"

#include "ShaderReader.h"

#include <algorithm>

FlockInitialiser::FlockInitialiser(Device& device, uint32_t flockSize, const std::string& shaderFolder, DescriptorPool& descriptorPool)
    : flockSize(flockSize)
    , groupCount(calcGroupCount())
    , device(device)
    , shader(device, shaderFolder)
    , descriptorPool(descriptorPool)
    , initPipeline(device, shader)
    , descriptorSets(device, descriptorPool, shader.bindings, {{nullptr, nullptr}})
{

}

uint32_t FlockInitialiser::calcGroupCount()
{

    return std::max(1u, (flockSize + LOCAL_SIZE - 1) / LOCAL_SIZE);
}

void FlockInitialiser::bindObjects(CommandBuffer& commandBuffer)
{

    initPipeline.bind(commandBuffer);

    descriptorSets[0].bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, initPipeline.getLayout());
}

void FlockInitialiser::initialise(CommandBuffer& commandBuffer, DeviceBuffer& posBuffer, DeviceBuffer& velBuffer)
{

    float cohesion{ 0.0 };

    descriptorSets = DescriptorSetPack(device, descriptorPool, shader.bindings, { {&posBuffer, &velBuffer} });

    bindObjects(commandBuffer);

    vkCmdPushConstants(commandBuffer.vk(), initPipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(uint32_t), &flockSize);

    //vkCmdPushConstants(commandBuffer.vk(), initPipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, sizeof(uint32_t), sizeof(float), &cohesion);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);
}
