#pragma once

#include "CommandBuffer.h"
#include "GLFWWindow.h"
#include "Device.h"
#include "VulkanInstance.h"
#include "SwapChain.h"
#include "UI.h"
#include "DescriptorPool.h"


struct UIRendererOptions
{

    const GLFWWindow& window;
    const Device& device;
    const VulkanInstance& instance;
    const SwapChain& swapChain;

    UIRendererOptions(
        const GLFWWindow& window,
        const Device& device,
        const VulkanInstance& instance,
        const SwapChain& swapChain
    )
        : window(window)
        , device(device)
        , instance(instance)
        , swapChain(swapChain)
    {}
};


class UIRenderer
{

    static const std::unordered_set<VkDescriptorType> requiredDescriptorTypes;
    DescriptorPool descriptorPool;

public:
    UIRenderer(const UIRendererOptions& options);

    void render(CommandBuffer& commandBuffer, UI& ui);

    ~UIRenderer();
};

