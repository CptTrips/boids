#include "SortShader.h"

#include "ShaderReader.h"

const std::string SortShader::shaderFileName = "sort.spv";

const std::vector<VkDescriptorSetLayoutBinding> SortShader::bindings;

const std::vector<VkPushConstantRange> SortShader::pushConstantRanges;

SortShader::SortShader(const Device& device, std::string shaderFolder)
	: Shader(
		device,
		ShaderReader(shaderFolder + shaderFileName).getCode(),
		VK_SHADER_STAGE_COMPUTE_BIT,
		bindings,
		pushConstantRanges
    )
{
}
