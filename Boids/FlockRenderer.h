#pragma once

#include "Renderer.h"
#include "ComputePipeline.h"
#include "Flock.h"
#include "BoidVertexShader.h"
#include "BoidFragmentShader.h"
#include "UIRenderer.h"
#include "BoidsUI.h"

class FlockRenderer
{

    static constexpr uint32_t INVOCATIONS{ 256 };

    static const std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

    DeviceBuffer indexBuffer;

    BoidVertexShader vertexShader;
    BoidFragmentShader fragmentShader;

    Renderer renderer;

    UIRenderer uiRenderer;

public:
    FlockRenderer(VulkanContext& context, uint32_t queueSize, const SwapChain& swapChain, std::string shaderFolder, uint32_t flockSize);

    void recordRenderCommands(CommandBuffer& commandBuffer, Flock& flock, const Image& image, UI& ui);
};

