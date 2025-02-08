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


FlockRenderer::FlockRenderer(VulkanContext& context, uint32_t queueSize, const SwapChain& swapChain, std::string shaderFolder, uint32_t flockSize)
    : indexBuffer(flockSize * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.device)
    , vertexShader(context.device, shaderFolder)
    , fragmentShader(context.device, shaderFolder)
    , renderer(RendererOptions(context, queueSize, swapChain, vertexShader, fragmentShader))
	, uiRenderer({ context.window, context.device, context.instance, swapChain.getFormat(), static_cast<uint32_t>(swapChain.getImageCount())})
{

	std::vector<uint32_t> indices(flockSize);

    for (uint32_t i{ 0 }; i < flockSize; i++)
        indices[i] = i;

	indexBuffer.upload(indices.data(), indices.size() * sizeof(uint32_t));
}

void FlockRenderer::recordRenderCommands(CommandBuffer& commandBuffer, Flock& flock, const Image& image, UI& ui)
{

    renderer.begin(commandBuffer, image);

    renderer.draw(commandBuffer, flock.getPositionBuffer(), indexBuffer);

    uiRenderer.render(commandBuffer, ui);

    renderer.end(commandBuffer, image);
}
