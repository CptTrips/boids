#pragma once

#include <string>

#include "VulkanContext.h"
#include "FlockRenderer.h"
#include "BoidsUI.h"
#include "Flock.h"

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

    VulkanContext context;

    FlockRenderer renderer;

    Flock flock;

    BoidsUI ui;


public:

    Application(ApplicationOptions);

    void run();

};

