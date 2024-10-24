#pragma once

#include <vector>

#include "DescriptorSet.h"
#include "DescriptorSetInfo.h"
#include "DescriptorPool.h"

class DescriptorSetPack
{

    std::vector<DescriptorSetLayout> descriptorSetLayouts;

    std::vector<DescriptorSet> descriptorSets;

    std::vector<DescriptorSetLayout> createDescriptorSetLayouts(const Device& device, const std::vector<std::vector<VkDescriptorSetLayoutBinding>>& descriptorSetLayoutBindings);

    std::vector<DescriptorSetInfo> makeDescriptorSetInfos(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const std::vector<std::vector<const DeviceBuffer*>>& buffers) const;

public:
    DescriptorSetPack(
        Device& device,
        DescriptorPool& descriptorPool,
        const std::vector<VkDescriptorSetLayoutBinding>& bindings,
        const std::vector<std::vector<const DeviceBuffer*>>& buffers
    );

    DescriptorSet& operator[](uint32_t i);

    const std::vector<DescriptorSetLayout>& getDescriptorSetLayouts() const;

    std::vector<VkDescriptorSetLayout> getDescriptorSetLayoutsVk() const;
};

