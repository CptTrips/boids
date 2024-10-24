#include "BoidInitShader.h"

#include "ShaderReader.h"

const std::string BoidInitShader::shaderFileName = "boidInit.spv";

const std::vector<VkPushConstantRange> BoidInitShader::pushConstantRanges
{
    {
        VK_SHADER_STAGE_COMPUTE_BIT,
        0,
        sizeof(uint32_t)
    }
};

static const VkDescriptorSetLayoutBinding posBinding
{

    0,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    1,
    VK_SHADER_STAGE_COMPUTE_BIT,
    nullptr
};

static const VkDescriptorSetLayoutBinding velBinding
{

    1,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    1,
    VK_SHADER_STAGE_COMPUTE_BIT,
    nullptr
};

const std::vector<VkDescriptorSetLayoutBinding> BoidInitShader::bindings{ posBinding, velBinding };

BoidInitShader::BoidInitShader(const Device& device, std::string shaderFolder)
    : Shader(
        device,
        ShaderReader(shaderFolder + shaderFileName).getCode(),
        VK_SHADER_STAGE_COMPUTE_BIT,
        bindings,
        pushConstantRanges
    )
{
}
