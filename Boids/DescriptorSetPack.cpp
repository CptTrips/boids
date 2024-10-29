#include "DescriptorSetPack.h"

std::vector<DescriptorSetLayout> DescriptorSetPack::createDescriptorSetLayouts(const Device& device, const std::vector<std::vector<VkDescriptorSetLayoutBinding>>& descriptorSetLayoutBindings)
{

    std::vector<DescriptorSetLayout> descriptorSetLayouts;

	for (const auto& bindings : descriptorSetLayoutBindings)
		descriptorSetLayouts.emplace_back(device, bindings);

    return descriptorSetLayouts;
}

std::vector<DescriptorSetInfo> DescriptorSetPack::makeDescriptorSetInfos(
    const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    const std::vector<std::vector<const DeviceBuffer*>>& buffers
) const
{

    std::vector<DescriptorSetInfo> descriptorSetInfos;

    for (uint32_t i{ 0 }; i < buffers.size(); i++)
    {

        std::vector<Descriptor> descriptors;

        for (uint32_t j{ 0 }; j < bindings.size(); j++)
        {

            std::vector<const DeviceBuffer*> buffer{ buffers[i][j] };

            descriptors.emplace_back(bindings[j], buffer);
        }

        descriptorSetInfos.emplace_back(
            descriptors,
            &descriptorSetLayout
        );

    }

    return descriptorSetInfos;
}

DescriptorSetPack::DescriptorSetPack(
    Device& device,
    DescriptorPool& descriptorPool,
    const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    const std::vector<std::vector<const DeviceBuffer*>>& buffers
)
    : descriptorSetLayout(device, bindings)
    , descriptorSets(descriptorPool.makeDescriptorSets(makeDescriptorSetInfos(bindings, buffers)))
{
}

DescriptorSet& DescriptorSetPack::operator[](uint32_t i)
{

    return descriptorSets[i];
}

const DescriptorSetLayout& DescriptorSetPack::getDescriptorSetLayout() const
{

	return descriptorSetLayout;
}

VkDescriptorSetLayout DescriptorSetPack::getDescriptorSetLayoutVk() const
{

    return descriptorSetLayout.vk();
}
