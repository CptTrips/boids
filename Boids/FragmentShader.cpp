#include "FragmentShader.h"

#include "ShaderReader.h"

const std::string FragmentShader::shaderFileName = "fragmentShader.spv";

const std::vector<VkDescriptorSetLayoutBinding> FragmentShader::bindings
{
	{
		1,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		1,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		nullptr
    }
};

const std::vector<VkPushConstantRange> FragmentShader::pushConstantRanges;

FragmentShader::FragmentShader(const Device& device, std::string shaderFolder)
    : Shader(
        device,
        ShaderReader(shaderFolder + shaderFileName).getCode(),
        VK_SHADER_STAGE_FRAGMENT_BIT,
        bindings,
        pushConstantRanges
    )
{
}
