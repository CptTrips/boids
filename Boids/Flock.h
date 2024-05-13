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

struct PushConstants
{

    uint32_t boidCount;
    float cohesion, alignment, separation;
};

class Flock
{

public:

    PushConstants parameters;

private:

    static constexpr uint32_t INVOCATIONS{ 256 };

    static const std::unordered_set<VkDescriptorType> requiredDescriptorTypes;

    static const VkDescriptorSetLayoutBinding inPosBinding, inVelBinding, outPosBinding, outVelBinding;

    static const VkPushConstantRange pushConstantRange;

    uint64_t step{ 0 };

    Device& device;

    DescriptorPool descriptorPool;

    uint32_t groupCount;

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

    Flock(Device& device, PushConstants parameters, const std::string& computeShaderPath, const std::string& initShaderPath);

    void update(CommandBuffer& commandBuffer);

    DeviceBuffer& getPositionBuffer();

    uint32_t getBoidCount() const;
};

