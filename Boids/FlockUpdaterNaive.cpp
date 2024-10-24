#include "FlockUpdaterNaive.h"

#include "ShaderReader.h"

std::vector<std::vector<const DeviceBuffer*>> FlockUpdaterNaive::makeBufferPointers(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const
{

    std::vector<std::vector<const DeviceBuffer*>> bufferPointers;

	for (uint32_t i{ 0 }; i < queueSize; i++)
	{

		bufferPointers.push_back({
			&posBuffers[i],
			&velBuffers[i],
			&posBuffers[(i + 1) % queueSize],
			&velBuffers[(i + 1) % queueSize],
        });
	}

	return bufferPointers;
}

void FlockUpdaterNaive::bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const
{

    pipeline.bind(commandBuffer);

	descriptorSet.bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.getLayout());
}

FlockUpdaterNaive::FlockUpdaterNaive(
	Device& device,
	const std::string& shaderFolder,
	DescriptorPool& descriptorPool,
	uint32_t boidCount,
    const std::vector<DeviceBuffer>& posBuffers,
    const std::vector<DeviceBuffer>& velBuffers,
    PushConstants parameters
)
    : FlockUpdater(device, posBuffers.size(), boidCount, LOCAL_SIZE, parameters)
	, shader(device, shaderFolder)
	, descriptorSets(device, descriptorPool, shader.bindings, makeBufferPointers(posBuffers, velBuffers))
	, pipeline(device, shader)
{
}

void FlockUpdaterNaive::update(CommandBuffer& commandBuffer, uint32_t step)
{

    bindObjects(commandBuffer, descriptorSets[step % queueSize]);

    vkCmdPushConstants(commandBuffer.vk(), pipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PushConstants), &parameters);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);
}
