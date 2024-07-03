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

    static constexpr float cohesion{ 1.f }, alignment{ 1.0f }, separation{ 1.0f };

    static constexpr uint32_t QUEUE_SIZE { 2 };

    VulkanContext context;

    SwapChain swapChain;

    FlockRenderer renderer;

    Flock flock;

    BoidsUI ui;

    std::vector<Fence> fences;

    std::vector<Semaphore> freeImageSemaphores, renderCompleteSemaphores;

    std::vector<CommandBuffer> commandBuffers;

    uint32_t frame{ 0 };

    GPUTimer computeTimer, drawTimer, frameTimer;

    PerformanceLog perfLog;

public:

    Application(ApplicationOptions);

    void run();

};

