#pragma once

#include <string>

#include "Shader.h"

class SortShader : public Shader
{

public:
    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

    SortShader(const Device& device, std::string shaderFolder);
};

