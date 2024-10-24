#include "SweepShader.h"

#include "ShaderReader.h"

const std::string SweepShader::shaderFileName = "sweep.spv";

const std::vector<VkDescriptorSetLayoutBinding> SweepShader::bindings;

const std::vector<VkPushConstantRange> SweepShader::pushConstantRanges;

SweepShader::SweepShader(const Device& device, std::string shaderFolder)
	: Shader(
		device,
		ShaderReader(shaderFolder + shaderFileName).getCode(),
		VK_SHADER_STAGE_COMPUTE_BIT,
		bindings,
		pushConstantRanges
    )
{
}
