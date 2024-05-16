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

	descriptorSets[step % QUEUE_SIZE].bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.getLayout());
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

	for (uint32_t i{ 0 }; i < QUEUE_SIZE; i++)
	{

		buffers.emplace_back(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);
	}

	return buffers;
}


void Flock::initBuffers()
{

	CommandBuffer commandBuffer{ device.makeSingleUseCommandBuffer() };

	initialiser.initialise(commandBuffer, posBuffers[0], velBuffers[0]);

	device.submitCommandBuffer(commandBuffer);

	device.graphicsQueueWaitIdle();
}

uint32_t Flock::calcGroupCount()
{

    return std::max(1u, (parameters.boidCount + LOCAL_SIZE - 1) / LOCAL_SIZE);
}

std::vector<DescriptorSetInfo> Flock::makeDescriptorSetInfos() const
{

    std::vector<DescriptorSetInfo> descriptorSetInfos;

	std::vector<Descriptor> descriptors;

	for (uint32_t i{ 0 }; i < QUEUE_SIZE; i++)
	{

        std::vector<const DeviceBuffer*> posBufferA{ &posBuffers[i] };

        std::vector<const DeviceBuffer*> velBufferA{ &velBuffers[i] };

        std::vector<const DeviceBuffer*> posBufferB{ &posBuffers[(i + 1) % QUEUE_SIZE]};

        std::vector<const DeviceBuffer*> velBufferB{ &velBuffers[(i + 1) % QUEUE_SIZE] };

        descriptors.emplace_back(inPosBinding, posBufferA);

        descriptors.emplace_back(inVelBinding, velBufferA);

        descriptors.emplace_back(outPosBinding, posBufferB);

        descriptors.emplace_back(outVelBinding, velBufferB);

        descriptorSetInfos.emplace_back(descriptors, &updateShaderDescriptorSetLayouts[0]);

        descriptors.clear();
	}

	return descriptorSetInfos;
}

std::vector<DescriptorSetLayout> Flock::makeUpdateShaderDescriptorSetLayouts() const
{

	std::vector<DescriptorSetLayout> layouts;

	std::vector<VkDescriptorSetLayoutBinding> bindings{ inPosBinding, inVelBinding, outPosBinding, outVelBinding };

	layouts.emplace_back(device, bindings);

	return layouts;
}


Flock::Flock(Device& device, PushConstants parameters, uint32_t queueSize, const std::string& computeShaderPath, const std::string& initShaderPath)
	: parameters{ parameters }
	, QUEUE_SIZE(queueSize)
	, device(device)
	, descriptorPool(device, requiredDescriptorTypes, 100)
	, groupCount(calcGroupCount())
	, posBuffers(makeBuffers())
	, velBuffers(makeBuffers())
	, updateShaderDescriptorSetLayouts(makeUpdateShaderDescriptorSetLayouts())
	, updateShader(device, ShaderReader(computeShaderPath).getCode(), VK_SHADER_STAGE_COMPUTE_BIT, updateShaderDescriptorSetLayouts, { pushConstantRange })
	, pipeline(device, updateShader)
	, descriptorSets(descriptorPool.makeDescriptorSets(makeDescriptorSetInfos()))
	, initialiser(device, parameters.boidCount, initShaderPath, descriptorPool)
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

	return posBuffers[step % QUEUE_SIZE];
}

uint32_t Flock::getBoidCount() const
{

	return parameters.boidCount;
}
