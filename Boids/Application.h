#pragma once

#include <string>
#include <vector>

#include "VulkanContext.h"
#include "FlockRenderer.h"
#include "BoidsUI.h"
#include "UIRenderer.h"
#include "Flock.h"

#include "Fence.h"
#include "GPUTimer.h"

#include "OptionsParser.h"

struct PerformanceLog
{

    std::vector<float> computeTimes, drawTimes, frameTimes;

    void log(float computeTime, float drawTime, float frameTime);

    void print() const;

private:
    static void rollingAppend(std::vector<float>& v, float x);
};

struct ApplicationOptions
{

    std::string shaderFolder;

    uint32_t flockSize;

    ApplicationOptions(OptionsParser& options)
        : shaderFolder(options.getShaderFolder())
        , flockSize(options.getFlockSize())
    {}
};

class Application
{

    static constexpr float initialCohesion{ 0.f }, initialAlignment{ 0.0f }, initialSeparation{ 0.0f }, initialPerceptionRange{ 0.05f }, initialSpeed{ 0.2f };

    static constexpr uint32_t QUEUE_SIZE { 2 };

    VulkanContext context;

    SwapChain swapChain;

    Flock flock;

    FlockRenderer renderer;
    
    std::vector<Fence> fences;

    std::vector<Semaphore> freeImageSemaphores, computeCompleteSemaphores, renderCompleteSemaphores;

    std::vector<CommandBuffer> computeCommandBuffers, renderCommandBuffers;

    uint32_t frame{ 0 };

    GPUTimer computeTimer, drawTimer, frameTimer;

    PerformanceLog perfLog;

    BoidsUI ui;

public:

    Application(ApplicationOptions);

    void run();

};

