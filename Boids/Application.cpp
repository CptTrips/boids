#include "Application.h"

#include "Vertex.h"

#include <chrono>
#include <thread>

Application::Application(ApplicationOptions options)
    : context()
	, swapChain(context.device, context.surface, context.window, QUEUE_SIZE + 2)
	, renderer(RendererOptions(context, QUEUE_SIZE, options.vertexShaderPath, options.fragmentShaderPath, swapChain), options.flockSize)
	, flock(context.device, { options.flockSize, 1.f, 1.f, 0.1f }, QUEUE_SIZE, options.computeShaderPath, options.initShaderPath)
	, ui({ &(flock.parameters.cohesion), &(flock.parameters.alignment), &(flock.parameters.separation) })
	, fences()
	, freeImageSemaphores()
	, renderCompleteSemaphores()
	, commandBuffers(context.device.makeCommandBuffers(QUEUE_SIZE, false))
{

	VkDevice device{ context.device.vk() };

	for (uint32_t i{ 0 }; i < QUEUE_SIZE; i++)
	{
		fences.emplace_back(device);

		freeImageSemaphores.emplace_back(device);

		renderCompleteSemaphores.emplace_back(device);
	}
}

void Application::run()
{

    while (!context.window.windowShouldClose())
    {

		uint32_t index{ frame % QUEUE_SIZE };

		Fence& fence{ fences[index] };

		fence.wait(UINT64_MAX);

		fence.reset();

		context.window.pollEvents();

        VkSemaphore freeImageSemaphore{ freeImageSemaphores[index].vk() };

        uint32_t imageIndex{ swapChain.getFreeImageIndex(VK_NULL_HANDLE, freeImageSemaphore)};

        Image image(swapChain.getImage(imageIndex));

        CommandBuffer& flockCommands{ commandBuffers[index]};

		flockCommands.reset();

		//context.device.writeTimestamp(VK_SHADER_STAGE_COMPUTE_BIT, frameStartQueries[index]);

        flock.update(flockCommands);

		renderer.recordRenderCommands(flockCommands, ui, flock, image);

		//context.device.writeTimestamp(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, frameEndQueries[index]);

        flockCommands.addWaitSemaphore(freeImageSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        flockCommands.addSignalSemaphore(renderCompleteSemaphores[index].vk());

        context.device.submitCommandBuffer(flockCommands, fence.vk());

        // Queue rendered frame
        std::vector<VkSemaphore> swapChainWaitSemaphores{ renderCompleteSemaphores[index].vk() };
        swapChain.queueImage(imageIndex, swapChainWaitSemaphores);

		frame++;

		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

