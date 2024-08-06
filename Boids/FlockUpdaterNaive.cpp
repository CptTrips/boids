#include "FlockUpdaterNaive.h"

#include "ShaderReader.h"

const VkDescriptorSetLayoutBinding FlockUpdaterNaive::inPosBinding
{

	0,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const VkDescriptorSetLayoutBinding FlockUpdaterNaive::inVelBinding
{

	1,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};
	
const VkDescriptorSetLayoutBinding FlockUpdaterNaive::outPosBinding
{

	2,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const VkDescriptorSetLayoutBinding FlockUpdaterNaive::outVelBinding
{

	3,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	1,
	VK_SHADER_STAGE_COMPUTE_BIT,
	nullptr
};

const std::vector<VkDescriptorSetLayoutBinding> FlockUpdaterNaive::bindings
{
	inPosBinding, inVelBinding, outPosBinding, outVelBinding
};

std::vector<DescriptorSetInfo> FlockUpdaterNaive::makeDescriptorSetInfos(const std::vector<DeviceBuffer>& posBuffers, const std::vector<DeviceBuffer>& velBuffers) const
{

    std::vector<DescriptorSetInfo> descriptorSetInfos;

	std::vector<Descriptor> descriptors;

	for (uint32_t i{ 0 }; i < queueSize; i++)
	{

        std::vector<const DeviceBuffer*> posBufferA{ &posBuffers[i] };

        std::vector<const DeviceBuffer*> velBufferA{ &velBuffers[i] };

        std::vector<const DeviceBuffer*> posBufferB{ &posBuffers[(i + 1) % queueSize]};

        std::vector<const DeviceBuffer*> velBufferB{ &velBuffers[(i + 1) % queueSize] };

        descriptors.emplace_back(inPosBinding, posBufferA);

        descriptors.emplace_back(inVelBinding, velBufferA);

        descriptors.emplace_back(outPosBinding, posBufferB);

        descriptors.emplace_back(outVelBinding, velBufferB);

        descriptorSetInfos.emplace_back(descriptors, shader.getDescriptorSetLayouts().data());

        descriptors.clear();
	}

	return descriptorSetInfos;
}

void FlockUpdaterNaive::bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const
{

    pipeline.bind(commandBuffer);

	descriptorSet.bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.getLayout());
}

FlockUpdaterNaive::FlockUpdaterNaive(
	Device& device,
	const std::string& computeShaderPath,
	DescriptorPool& descriptorPool,
	uint32_t boidCount,
    const std::vector<DeviceBuffer>& posBuffers,
    const std::vector<DeviceBuffer>& velBuffers,
    PushConstants parameters
)
    : FlockUpdater(device, posBuffers.size(), boidCount, LOCAL_SIZE, parameters)
	, shader(device, ShaderReader(computeShaderPath).getCode(), VK_SHADER_STAGE_COMPUTE_BIT, { bindings }, { parameters.range })
	, pipeline(device, shader)
	, descriptorSets(descriptorPool.makeDescriptorSets(makeDescriptorSetInfos(posBuffers, velBuffers)))
{
}

void FlockUpdaterNaive::update(CommandBuffer& commandBuffer, uint32_t step)
{

    bindObjects(commandBuffer, descriptorSets[step % queueSize]);

    vkCmdPushConstants(commandBuffer.vk(), pipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PushConstants), &parameters);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);
}
