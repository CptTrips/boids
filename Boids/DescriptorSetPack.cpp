#include "DescriptorSetPack.h"

std::vector<DescriptorSetLayout> DescriptorSetPack::createDescriptorSetLayouts(const Device& device, const std::vector<std::vector<VkDescriptorSetLayoutBinding>>& descriptorSetLayoutBindings)
{

    std::vector<DescriptorSetLayout> descriptorSetLayouts;

	for (const auto& bindings : descriptorSetLayoutBindings)
		descriptorSetLayouts.emplace_back(device, bindings);

    return descriptorSetLayouts;
}

std::vector<DescriptorSetInfo> DescriptorSetPack::makeDescriptorSetInfos(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const std::vector<std::vector<const DeviceBuffer*>>& buffers) const
{

    std::vector<DescriptorSetInfo> descriptorSetInfos;

    std::vector<Descriptor> descriptors;

    for (uint32_t i{ 0 }; i < buffers.size(); i++)
    {

        for (uint32_t j{ 0 }; j < bindings.size(); j++)
        {

            std::vector<const DeviceBuffer*> buffer{ buffers[i][j] };

            descriptors.emplace_back(bindings[j], buffer);
        }

    }

    return descriptorSetInfos;
}

DescriptorSetPack::DescriptorSetPack(
    Device& device,
    DescriptorPool& descriptorPool,
    const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    const std::vector<std::vector<const DeviceBuffer*>>& buffers
)
    : descriptorSetLayouts(createDescriptorSetLayouts(device, { bindings }))
    , descriptorSets(descriptorPool.makeDescriptorSets(makeDescriptorSetInfos(bindings, buffers)))
{
}

DescriptorSet& DescriptorSetPack::operator[](uint32_t i)
{

    return descriptorSets[i];
}

const std::vector<DescriptorSetLayout>& DescriptorSetPack::getDescriptorSetLayouts() const
{

	return descriptorSetLayouts;
}

std::vector<VkDescriptorSetLayout> DescriptorSetPack::getDescriptorSetLayoutsVk() const
{

	std::vector<VkDescriptorSetLayout> layouts;

	for (size_t i{ 0 }; i < descriptorSetLayouts.size(); i++)
		layouts.push_back(descriptorSetLayouts[i].vk());

	return layouts;
}
