#include "ComputeWork.h"

#include "ShaderReader.h"

void ComputeWork::bindObjects(CommandBuffer& commandBuffer, DescriptorSet& descriptorSet) const
{

    pipeline.bind(commandBuffer);

	descriptorSet.bind(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline.getLayout());
}

ComputeWork::ComputeWork(
    Device& device,
    DescriptorPool& descriptorPool,
    uint32_t queueSize,
    uint32_t groupCount,
    uint32_t localSize,
    PushConstants parameters,
    const Shader& shader,
    std::vector<std::vector<const DeviceBuffer*>> buffers
)
	: device(device)
	, queueSize(queueSize)
    , localSize(localSize)
	, groupCount(groupCount)
	, parameters(parameters)
	, descriptorSets(device, descriptorPool, shader.bindings, buffers)
    , pipeline(device, shader)
{
}

void ComputeWork::dispatch(CommandBuffer& commandBuffer, uint32_t step)
{

    bindObjects(commandBuffer, descriptorSets[step % queueSize]);

    vkCmdPushConstants(commandBuffer.vk(), pipeline.getLayout().vk(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PushConstants), &parameters);

    vkCmdDispatch(commandBuffer.vk(), groupCount, 1, 1);
}
