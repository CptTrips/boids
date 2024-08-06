#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

struct PushConstants
{

    static const VkPushConstantRange range;

    uint32_t boidCount;
    float cohesion, alignment, separation;
};

