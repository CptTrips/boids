#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "Device.h"
#include "Shader.h"

class FragmentShader : public Shader
{

    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

public:
    FragmentShader(const Device& device, std::string shaderFolder);
    
};

