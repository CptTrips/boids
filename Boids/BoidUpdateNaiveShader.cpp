#include "BoidUpdateNaiveShader.h"

#include "ShaderReader.h"

const std::string BoidUpdateNaiveShader::shaderFileName = "boidUpdate3D.spv";

static const VkDescriptorSetLayoutBinding inPosBinding
{

	0,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

static const VkDescriptorSetLayoutBinding inVelBinding
{

	1,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};
	
static const VkDescriptorSetLayoutBinding outPosBinding
{

	2,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

static const VkDescriptorSetLayoutBinding outVelBinding
{

	3,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const std::vector<VkDescriptorSetLayoutBinding> BoidUpdateNaiveShader::bindings
{
	inPosBinding, inVelBinding, outPosBinding, outVelBinding
};

const std::vector<VkPushConstantRange> BoidUpdateNaiveShader::pushConstantRanges;

BoidUpdateNaiveShader::BoidUpdateNaiveShader(const Device & device, std::string shaderFolder)
	: Shader(
		device,
		ShaderReader(shaderFolder + shaderFileName).getCode(),
		VK_SHADER_STAGE_COMPUTE_BIT,
		bindings,
		pushConstantRanges
    )
{
}
