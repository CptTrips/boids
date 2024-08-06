#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "VulkanContext.h"
#include "DeviceBuffer.h"
#include "VulkanInstance.h"
#include "GLFWWindow.h"
#include "Surface.h"
#include "Device.h"
#include "SwapChain.h"
#include "UI.h"
#include "UIRenderer.h"
#include "PipelineBarrier.h"
#include "Image.h"
#include "GraphicsPipeline.h"
#include "VertexShader.h"
#include "DescriptorSetLayout.h"
#include "Semaphore.h"

struct RendererOptions
{

    GLFWWindow& window;

    VulkanInstance& instance;

    Surface& surface;

    Device& device;

    uint32_t queueSize;

    std::string vertexShaderPath, fragmentShaderPath;

    VkFormat swapChainFormat;

    uint32_t swapChainSize;

    VkExtent2D swapChainExtent;

    RendererOptions(
        VulkanContext& context
        , uint32_t queueSize
        , std::string vertexShaderPath
        , std::string fragmentShaderPath
        , const SwapChain& swapChain
    )
        : window(context.window)
        , instance(context.instance)
        , surface(context.surface)
        , device(context.device)
        , queueSize(queueSize)
        , vertexShaderPath(vertexShaderPath)
        , fragmentShaderPath(fragmentShaderPath)
        , swapChainFormat(swapChain.getFormat())
        , swapChainSize(static_cast<uint32_t>(swapChain.getImageCount()))
        , swapChainExtent(swapChain.getExtent())
    {}
};

class Renderer
{

protected:

    Device& device;

private:

    static const std::vector<PipelineStage> pipelineStages;

    static const std::vector<VkDescriptorSetLayoutBinding> vertexDescriptorBindings, fragmentDescriptorBindings;

    static const std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;

    static const std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

    const uint32_t QUEUE_SIZE;

    const std::vector<PipelineBarrier> pipelineBarriers;

    UIRenderer uiRenderer;

    VertexShader vertexShader;
    Shader fragmentShader;

    GraphicsPipeline graphicsPipeline;

    //std::unique_ptr<DeviceBuffer> vertexBuffer, indexBuffer;

    VkExtent2D renderDomain;

    std::vector<PipelineBarrier> createPipelineBarriers() const;

    void beginRendering(CommandBuffer& commandBuffer, const Image& image, VkRect2D renderDomain) const;

    void bindObjects(CommandBuffer& commandBuffer, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer) const;

    void setDomain(CommandBuffer& commandBuffer, VkExtent2D extent);

    void drawIndexed(CommandBuffer& commandBuffer, uint32_t indexCount) const;

public:

    Renderer(RendererOptions options);

    void recordRenderCommands(CommandBuffer& commandBuffer, UI& ui, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer, const Image & image);
};

