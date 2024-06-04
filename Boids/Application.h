#pragma once

#include <string>
#include <vector>

#include "VulkanContext.h"
#include "FlockRenderer.h"
#include "BoidsUI.h"
#include "Flock.h"

#include "Fence.h"
#include "GPUTimer.h"

#include "OptionsParser.h"

struct ApplicationOptions
{

    std::string vertexShaderPath, fragmentShaderPath, computeShaderPath, initShaderPath;

    uint32_t flockSize;

    ApplicationOptions(OptionsParser& options)
        : vertexShaderPath(options.getVertexShaderPath())
        , fragmentShaderPath(options.getFragmentShaderPath())
        , computeShaderPath(options.getComputeShaderPath())
        , initShaderPath(options.getInitShaderPath())
        , flockSize(options.getFlockSize())
    {}
};

class Application
{

    static constexpr uint32_t QUEUE_SIZE { 1 };

    VulkanContext context;

    SwapChain swapChain;

    FlockRenderer renderer;

    Flock flock;

    BoidsUI ui;

    std::vector<Fence> fences;

    std::vector<Semaphore> freeImageSemaphores, renderCompleteSemaphores;

    std::vector<CommandBuffer> commandBuffers;

    uint32_t frame{ 0 };

    GPUTimer timer;

public:

    Application(ApplicationOptions);

    void run();

};

