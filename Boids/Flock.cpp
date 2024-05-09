#include "Flock.h"
#include "Vertex.h"
#include "ShaderReader.h"

#include <array>
#include <algorithm>

const std::unordered_set<VkDescriptorType> Flock::requiredDescriptorTypes{
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
};

const VkDescriptorSetLayoutBinding Flock::inPosBinding
{

	0,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const VkDescriptorSetLayoutBinding Flock::inVelBinding
{

	1,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};
	
const VkDescriptorSetLayoutBinding Flock::outPosBinding
{

	2,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const VkDescriptorSetLayoutBinding Flock::outVelBinding
{

	3,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};
	
const VkPushConstantRange Flock::pushConstantRange
{
	VK_SHADER_STAGE_COMPUTE_BIT,
	0,
	sizeof(uint32_t) + 3 * sizeof(float)
};

void Flock::bindObjects(CommandBuffer& commandBuffer) const
{

    pipeline.bind(commandBuffer);

	descriptorSets[step % 2].bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.getLayout());
}

std::vector<DeviceBuffer> Flock::makeBuffers() const
{

	VkDeviceSize bufferSize{ parameters.boidCount * sizeof(glm::vec2) };

	VkBufferUsageFlags usage{
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
	};

	std::vector<DeviceBuffer> buffers;

	buffers.emplace_back(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);

	buffers.emplace_back(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);

	return buffers;
}


void Flock::initBuffers()
{

	CommandBuffer commandBuffer{ device.makeSingleUseCommandBuffer() };

	initialiser.initialise(commandBuffer, posBuffers[0], velBuffers[0]);

	device.submitCommandBuffer(commandBuffer);

	device.graphicsQueueWaitIdle();
}

uint32_t Flock::calcGroupCount(uint32_t boidCount)
{

    return std::max(1u, boidCount / INVOCATIONS);
}

std::vector<DescriptorSetInfo> Flock::makeDescriptorSetInfos() const
{

    std::vector<DescriptorSetInfo> descriptorSetInfos;

	std::vector<Descriptor> descriptors;

	std::vector<const DeviceBuffer*> posBufferA{ &posBuffers[0] };

	std::vector<const DeviceBuffer*> velBufferA{ &velBuffers[0] };

	std::vector<const DeviceBuffer*> posBufferB{ &posBuffers[1] };

	std::vector<const DeviceBuffer*> velBufferB{ &velBuffers[1] };

	descriptors.emplace_back(inPosBinding, posBufferA);

	descriptors.emplace_back(inVelBinding, velBufferA);

	descriptors.emplace_back(outPosBinding, posBufferB);

	descriptors.emplace_back(outVelBinding, velBufferB);

    descriptorSetInfos.emplace_back(descriptors, &updateShaderDescriptorSetLayouts[0]);

	descriptors.clear();

	descriptors.emplace_back(inPosBinding, posBufferB);

	descriptors.emplace_back(inVelBinding, velBufferB);

	descriptors.emplace_back(outPosBinding, posBufferA);

	descriptors.emplace_back(outVelBinding, velBufferA);

    descriptorSetInfos.emplace_back(descriptors, &updateShaderDescriptorSetLayouts[0]);

	return descriptorSetInfos;
}

std::vector<DescriptorSetLayout> Flock::makeUpdateShaderDescriptorSetLayouts() const
{

	std::vector<DescriptorSetLayout> layouts;

	std::vector<VkDescriptorSetLayoutBinding> bindings{ inPosBinding, inVelBinding, outPosBinding, outVelBinding };

	layouts.emplace_back(device, bindings);

	return layouts;
}


Flock::Flock(Device& device, uint32_t boidCount, const std::string& computeShaderPath, const std::string& initShaderPath)
	: parameters{ boidCount, 2.f, 1.f, 1.f }
	, device(device)
	, descriptorPool(device, requiredDescriptorTypes, 100)
	, groupCount(calcGroupCount(boidCount))
	, posBuffers(makeBuffers())
	, velBuffers(makeBuffers())
	, updateShaderDescriptorSetLayouts(makeUpdateShaderDescriptorSetLayouts())
	, updateShader(device, ShaderReader(computeShaderPath).getCode(), VK_SHADER_STAGE_COMPUTE_BIT, updateShaderDescriptorSetLayouts, { pushConstantRange })
	, pipeline(device, updateShader)
	, descriptorSets(descriptorPool.makeDescriptorSets(makeDescriptorSetInfos()))
	, initialiser(device, boidCount, initShaderPath, descriptorPool)
{

	initBuffers();
}

void Flock::update(CommandBuffer& commandBuffer)
{

    bindObjects(commandBuffer);

    vkCmdPushConstants(commandBuffer.vk(), pipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PushConstants), &parameters);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);

	step++;
}

DeviceBuffer& Flock::getPositionBuffer()
{

	return posBuffers[step % 2];
}

uint32_t Flock::getBoidCount() const
{

	return parameters.boidCount;
}
