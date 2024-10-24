#include "FlockUpdaterSweepAndPrune.h"

#include "ShaderReader.h"

std::vector<DeviceBuffer> FlockUpdaterSweepAndPrune::makeIndexBuffers(uint32_t boidCount) const
{

	VkDeviceSize bufferSize{ boidCount * sizeof(uint32_t)};

	VkBufferUsageFlags usage{
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_TRANSFER_DST_BIT
	};

    std::vector<DeviceBuffer> buffers;

    for (int i{ 0 }; i < queueSize; i++)
        buffers.emplace_back(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);

    return buffers;
}

std::vector<DeviceBuffer> FlockUpdaterSweepAndPrune::makeInteractionBuffers(uint32_t boidCount) const
{

	VkDeviceSize bufferSize{ boidCount * boidCount * sizeof(bool)};

	VkBufferUsageFlags usage{
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_TRANSFER_DST_BIT
	};

    std::vector<DeviceBuffer> buffers;

    for (int i{ 0 }; i < queueSize; i++)
        buffers.emplace_back(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);

    return buffers;
}

std::vector<std::vector<const DeviceBuffer*>> FlockUpdaterSweepAndPrune::sortBufferTable(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const
{
    return std::vector<std::vector<const DeviceBuffer*>>();
}

std::vector<std::vector<const DeviceBuffer*>> FlockUpdaterSweepAndPrune::sweepBufferTable() const
{
    return std::vector<std::vector<const DeviceBuffer*>>();
}

std::vector<std::vector<const DeviceBuffer*>> FlockUpdaterSweepAndPrune::interactionBufferTable(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const
{
    return std::vector<std::vector<const DeviceBuffer*>>();
}

FlockUpdaterSweepAndPrune::FlockUpdaterSweepAndPrune(
    Device& device,
    const std::string& shaderFolder,
    DescriptorPool& descriptorPool,
    uint32_t boidCount,
    const std::vector<DeviceBuffer>& posBuffers,
    const std::vector<DeviceBuffer>& velBuffers,
    PushConstants parameters
)
    : FlockUpdater(device, posBuffers.size(), boidCount, LOCAL_SIZE, parameters)
    , sortedIndicesX(makeIndexBuffers(boidCount))
    , sortedIndicesY(makeIndexBuffers(boidCount))
    , sortedIndicesZ(makeIndexBuffers(boidCount))
    , interactions(makeInteractionBuffers(boidCount))
    , sortShader(device, shaderFolder)
    , sweepShader(device, shaderFolder)
    , interactionShader(device, shaderFolder)
	, sortWork(device, descriptorPool, posBuffers.size(), calcGroupCount(boidCount, LOCAL_SIZE), LOCAL_SIZE, parameters, sortShader, sortBufferTable(posBuffers, velBuffers))
	, sweepWork(device, descriptorPool, posBuffers.size(), calcGroupCount(boidCount, LOCAL_SIZE), LOCAL_SIZE, parameters, sweepShader, sweepBufferTable())
	, interactionWork(device, descriptorPool, posBuffers.size(), calcGroupCount(boidCount, LOCAL_SIZE), LOCAL_SIZE, parameters, interactionShader, interactionBufferTable(posBuffers, velBuffers))
{
}

void FlockUpdaterSweepAndPrune::update(CommandBuffer& commandBuffer, uint32_t step)
{
}
