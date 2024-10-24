#pragma once

#include "FlockUpdater.h"
#include "SortShader.h";
#include "SweepShader.h"
#include "InteractionShader.h"
#include "ComputeWork.h"
#include "DescriptorPool.h"
#include "PushConstants.h"

class FlockUpdaterSweepAndPrune : public FlockUpdater
{
    static constexpr uint32_t LOCAL_SIZE{ 128 };

    std::vector<DeviceBuffer> sortedIndicesX, sortedIndicesY, sortedIndicesZ, interactions;

    SortShader sortShader;
    SweepShader sweepShader;
    InteractionShader interactionShader;

    ComputeWork sortWork, sweepWork, interactionWork;

    std::vector<DeviceBuffer> makeIndexBuffers(uint32_t boidCount) const;

    std::vector<DeviceBuffer> makeInteractionBuffers(uint32_t boidCount) const;

    std::vector<std::vector<const DeviceBuffer*>> sortBufferTable(
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers
    ) const;

    std::vector<std::vector<const DeviceBuffer*>> sweepBufferTable() const;

    std::vector<std::vector<const DeviceBuffer*>> interactionBufferTable(
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers
    ) const;

public:

    PushConstants parameters;

    FlockUpdaterSweepAndPrune(
        Device& device,
        const std::string& shaderFolder,
        DescriptorPool& descriptorPool,
        uint32_t boidCount,
        const std::vector<DeviceBuffer>& posBuffers,
        const std::vector<DeviceBuffer>& velBuffers,
        PushConstants parameters
    );

    virtual void update(CommandBuffer& commandBuffer, uint32_t step);
};

