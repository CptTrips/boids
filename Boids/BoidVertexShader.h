#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include <string>

#include "VertexShader.h"

using glm::vec4;

class BoidVertexShader : public VertexShader
{

    static const std::string shaderFileName;

    static const std::vector<VkDescriptorSetLayoutBinding> bindings;

    static const std::vector<VkPushConstantRange> pushConstantRanges;

    static const std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;

    static const std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

public:
    BoidVertexShader(const Device& device, std::string shaderFolder);
};

