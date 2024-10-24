#pragma once

#include <string>

#include "Shader.h"

class SweepShader : public Shader
{

public:
    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

    SweepShader(const Device& device, std::string shaderFolder);
};

