#pragma once
#include "FragmentShader.h"

class BoidFragmentShader :
    public FragmentShader
{

    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

public:
    BoidFragmentShader(const Device& device, std::string shaderFolder);
};

