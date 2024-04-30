#include "FlockRenderer.h"

#include <array>

const std::vector<VkDescriptorSetLayoutBinding> FlockRenderer::layoutBindings
{
    {
        0,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        2,
        VK_SHADER_STAGE_COMPUTE_BIT,
        nullptr
    }
};

void FlockRenderer::bindObjects(CommandBuffer& commandBuffer)
{
}

void FlockRenderer::updateVertices()
{

    /*
    CommandBuffer commandBuffer{ device.makeSingleUseCommandBuffer() };

    bindObjects(commandBuffer);

    vkCmdDispatch(commandBuffer.vk(), static_cast<uint32_t>(flock.getBoidCount() / INVOCATIONS), 1, 1);

    device.submitCommandBuffer(commandBuffer);

    device.graphicsQueueWaitIdle();
    */
}

FlockRenderer::FlockRenderer(RendererOptions options, uint32_t flockSize)
    : Renderer(options)
    , indexBuffer(flockSize * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, options.device)
{

	std::vector<uint32_t> indices(flockSize);

    for (uint32_t i{ 0 }; i < flockSize; i++)
        indices[i] = i;

	indexBuffer.upload(indices.data(), indices.size() * sizeof(uint32_t));
}

void FlockRenderer::render(UI& ui, Flock& flock)
{

    updateVertices();

    Renderer::render(ui, flock.getPositionBuffer(), indexBuffer);
}
