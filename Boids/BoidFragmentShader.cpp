#include "BoidFragmentShader.h"

#include "ShaderReader.h"

const std::string BoidFragmentShader::shaderFileName = "fragmentShader.spv";

const std::vector<VkDescriptorSetLayoutBinding> BoidFragmentShader::bindings
{
	{
		1,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		1,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		nullptr
    }
};

const std::vector<VkPushConstantRange> BoidFragmentShader::pushConstantRanges;


BoidFragmentShader::BoidFragmentShader(const Device& device, std::string shaderFolder)
    : FragmentShader(
        device,
        ShaderReader(shaderFolder + shaderFileName).getCode(),
        bindings,
        pushConstantRanges
    )
{
}
