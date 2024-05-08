#pragma once

#include <glm/glm.hpp>

#include "CommandBuffer.h"
#include "ComputePipeline.h"
#include "DeviceBuffer.h"

#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetInfo.h"
#include "FlockInitialiser.h"

struct Boid
{

    glm::vec2 position;
    glm::vec2 velocity;
};

class Flock
{

    static constexpr uint32_t INVOCATIONS{ 256 };

    static const std::unordered_set<VkDescriptorType> requiredDescriptorTypes;

    static const VkDescriptorSetLayoutBinding inPosBinding, inVelBinding, outPosBinding, outVelBinding;

    static const VkPushConstantRange pushConstantRange;

    uint64_t step{ 0 };

    float cohesion, alignment, separation;

    Device& device;

    DescriptorPool descriptorPool;

    uint32_t boidCount, groupCount;

    std::vector<DeviceBuffer> posBuffers;

    std::vector<DeviceBuffer> velBuffers;

    std::vector<DescriptorSetLayout> updateShaderDescriptorSetLayouts;

    Shader updateShader;

    ComputePipeline pipeline;

    std::vector<DescriptorSet> descriptorSets;

    FlockInitialiser initialiser;

    void bindObjects(CommandBuffer& commandBuffer) const;

    std::vector<DeviceBuffer> makeBuffers() const;

    void initBuffers();

    uint32_t calcGroupCount();

    std::vector<DescriptorSetInfo> makeDescriptorSetInfos() const;

    std::vector<DescriptorSetLayout> makeUpdateShaderDescriptorSetLayouts() const;

public:
    Flock(Device& device, uint32_t boidCount, const std::string& computeShaderPath, const std::string& initShaderPath);

    void update(CommandBuffer& commandBuffer);

    DeviceBuffer& getPositionBuffer();

    uint32_t getBoidCount() const;

    void setCohesion(CommandBuffer& commandBuffer, float cohesion);

    void setAlignment(CommandBuffer& commandBuffer, float alignment);

    void setSeparation(CommandBuffer& commandBuffer, float separation);
};

