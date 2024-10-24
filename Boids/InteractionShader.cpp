#include "InteractionShader.h"

#include "ShaderReader.h"

const std::string InteractionShader::shaderFileName = "interaction.spv";

const std::vector<VkDescriptorSetLayoutBinding> InteractionShader::bindings;

const std::vector<VkPushConstantRange> InteractionShader::pushConstantRanges;

InteractionShader::InteractionShader(const Device& device, std::string shaderFolder)
	: Shader(
		device,
		ShaderReader(shaderFolder + shaderFileName).getCode(),
		VK_SHADER_STAGE_COMPUTE_BIT,
		bindings,
		pushConstantRanges
    )
{
}
