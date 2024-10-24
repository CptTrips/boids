#include "FlockUpdaterSweepAndPrune.h"

#include "ShaderReader.h"

std::vector<DeviceBuffer> FlockUpdaterSweepAndPrune::makeIndexBuffers() const
{
    return std::vector<DeviceBuffer>();
}

std::vector<DeviceBuffer> FlockUpdaterSweepAndPrune::makeInteractionBuffers() const
{
    return std::vector<DeviceBuffer>();
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
    , sortedIndicesX(makeIndexBuffers())
    , sortedIndicesY(makeIndexBuffers())
    , sortedIndicesZ(makeIndexBuffers())
    , interactions(makeInteractionBuffers())
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
