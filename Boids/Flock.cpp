#include "Flock.h"
#include "Vertex.h"

#include <array>
#include <algorithm>

const std::unordered_set<VkDescriptorType> Flock::requiredDescriptorTypes{
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
};

	
std::vector<DeviceBuffer> Flock::makeBuffers(VkDeviceSize elementSize, VkBufferUsageFlags extraUsageFlags) const
{

	VkDeviceSize bufferSize{ parameters.boidCount * elementSize };

	VkBufferUsageFlags usage{
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| extraUsageFlags
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



Flock::Flock(Device& device, PushConstants parameters, uint32_t queueSize, const std::string& shaderFolder)
	: parameters{ parameters }
	, QUEUE_SIZE(queueSize)
	, device(device)
	, descriptorPool(device, requiredDescriptorTypes, 100)
	, posBuffers(makeBuffers(sizeof(glm::vec4), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT))
	, velBuffers(makeBuffers(sizeof(glm::vec4), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT))
	, initialiser(device, parameters.boidCount, shaderFolder, descriptorPool)
	, updater(device, shaderFolder, descriptorPool, parameters.boidCount, posBuffers, velBuffers, parameters)
{

	initBuffers();
}

void Flock::update(CommandBuffer& commandBuffer)
{

	updater.update(commandBuffer, step);

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
