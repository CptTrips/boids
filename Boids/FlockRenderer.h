#pragma once

#include "Renderer.h"
#include "ComputePipeline.h"
#include "Flock.h"

class FlockRenderer :
    public Renderer
{

    static constexpr uint32_t INVOCATIONS{ 256 };

    static const std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

    DeviceBuffer indexBuffer;

    void bindObjects(CommandBuffer& commandBuffer);

    void updateVertices();

public:
    FlockRenderer(RendererOptions options, uint32_t flockSize);

    void recordRenderCommands(CommandBuffer& commandBuffer, UI& ui, Flock& flock, const Image& image);
};

