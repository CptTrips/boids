#pragma once

#include <string>

#include "Shader.h"

class InteractionShader : public Shader
{

public:
    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

    InteractionShader(const Device& device, std::string shaderFolder);
};

