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

    size_t flockSize;

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

    BoidsUI ui;

    FlockRenderer renderer;

    Flock flock;

    void processUIEvents();


public:

    Application(ApplicationOptions);

    void run();

};

