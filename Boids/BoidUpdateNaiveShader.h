#pragma once

#include <string>

#include "Shader.h"

class BoidUpdateNaiveShader : public Shader
{

public:
    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

    BoidUpdateNaiveShader(const Device& device, std::string shaderFolder);
};

