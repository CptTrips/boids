#pragma once

#include "Renderer.h"
#include "ComputePipeline.h"
#include "Flock.h"
#include "BoidVertexShader.h"
#include "BoidFragmentShader.h"

class FlockRenderer
{

    static constexpr uint32_t INVOCATIONS{ 256 };

    static const std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

    DeviceBuffer indexBuffer;

    BoidVertexShader vertexShader;
    BoidFragmentShader fragmentShader;

    Renderer renderer;

    void bindObjects(CommandBuffer& commandBuffer);

    void updateVertices();

public:
    FlockRenderer(VulkanContext& context, uint32_t queueSize, const SwapChain& swapChain, std::string shaderFolder, uint32_t flockSize);

    void recordRenderCommands(CommandBuffer& commandBuffer, UI& ui, Flock& flock, const Image& image);
};

