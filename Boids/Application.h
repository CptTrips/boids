#pragma once

#include <string>
#include <vector>

#include "VulkanContext.h"
#include "FlockRenderer.h"
#include "BoidsUI.h"
#include "Flock.h"
#include "Fence.h"

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

    static constexpr uint32_t QUEUE_SIZE { 2 };

    VulkanContext context;

    FlockRenderer renderer;

    Flock flock;

    BoidsUI ui;

    std::vector<Fence> fences;

    uint32_t frame{ 0 };

public:

    Application(ApplicationOptions);

    void run();

};

