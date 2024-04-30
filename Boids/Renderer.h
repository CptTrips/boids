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

struct RendererOptions
{

    GLFWWindow& window;

    VulkanInstance& instance;

    Surface& surface;

    Device& device;

    std::string vertexShaderPath, fragmentShaderPath;

    RendererOptions(
        VulkanContext& context
        , std::string vertexShaderPath
        , std::string fragmentShaderPath
    )
        : window(context.window)
        , instance(context.instance)
        , surface(context.surface)
        , device(context.device)
        , vertexShaderPath(vertexShaderPath)
        , fragmentShaderPath(fragmentShaderPath)
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

    SwapChain swapChain;

    const std::vector<PipelineBarrier> pipelineBarriers;

    UIRenderer uiRenderer;

    std::vector<DescriptorSetLayout> vertexDescriptorSetLayouts, fragmentDescriptorSetLayouts;

    VertexShader vertexShader;
    Shader fragmentShader;

    GraphicsPipeline graphicsPipeline;

    //std::unique_ptr<DeviceBuffer> vertexBuffer, indexBuffer;

    std::vector<PipelineBarrier> createPipelineBarriers() const;

    void recordRenderCommands(CommandBuffer& commandBuffer, Image & image, UI& ui, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer);

    void beginRendering(CommandBuffer& commandBuffer, const Image& image) const;

    void bindObjects(CommandBuffer& commandBuffer, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer) const;

    void drawIndexed(CommandBuffer& commandBuffer, uint32_t indexCount) const;

    std::vector<DescriptorSetLayout> makeVertexDescriptorSetLayouts() const;

    std::vector<DescriptorSetLayout> makeFragmentDescriptorSetLayouts() const;

public:

    Renderer(RendererOptions options);

    void render(UI& ui, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer);
};

