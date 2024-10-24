#include "BoidVertexShader.h"

#include "ShaderReader.h"

const std::string BoidVertexShader::shaderFileName = "vertexShader.spv";

const std::vector<VkDescriptorSetLayoutBinding> BoidVertexShader::bindings
{
    {
        0,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        1,
        VK_SHADER_STAGE_VERTEX_BIT,
        nullptr
    }
};

const std::vector<VkPushConstantRange> BoidVertexShader::pushConstantRanges;

const std::vector<VkVertexInputBindingDescription> BoidVertexShader::vertexInputBindingDescriptions
{

	{ 0, sizeof(vec4), VK_VERTEX_INPUT_RATE_VERTEX }
};

const std::vector<VkVertexInputAttributeDescription> BoidVertexShader::vertexAttributeDescriptions
{

	{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}
};


BoidVertexShader::BoidVertexShader(const Device& device, std::string shaderFolder)
    : VertexShader(
        device,
        ShaderReader(shaderFolder + shaderFileName).getCode(),
        VK_SHADER_STAGE_VERTEX_BIT,
        bindings,
        pushConstantRanges,
        vertexInputBindingDescriptions,
        vertexAttributeDescriptions
    )
{
}
