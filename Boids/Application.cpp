#include "Application.h"

#include "Vertex.h"

#include <chrono>
#include <thread>

Application::Application(ApplicationOptions options)
    : context()
	, swapChain(context.device, context.surface, context.window)
	, renderer(RendererOptions(context, QUEUE_SIZE, options.vertexShaderPath, options.fragmentShaderPath, swapChain), options.flockSize)
	, flock(context.device, { options.flockSize, 1.f, 1.f, 0.1f }, QUEUE_SIZE, options.computeShaderPath, options.initShaderPath)
	, ui({ &(flock.parameters.cohesion), &(flock.parameters.alignment), &(flock.parameters.separation) })
	, fences()
	, freeImageSemaphores()
	, renderCompleteSemaphores()
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

		Fence& fence{ fences[frame % QUEUE_SIZE ] };

		fence.wait(UINT64_MAX);

		fence.reset();

		context.window.pollEvents();

        VkSemaphore freeImageSemaphore{ freeImageSemaphores[frame % QUEUE_SIZE].vk() };

        uint32_t imageIndex{ swapChain.getFreeImageIndex(VK_NULL_HANDLE, freeImageSemaphore)};

        Image image(swapChain.getImage(imageIndex));

        CommandBuffer flockCommands{ context.device.makeSingleUseCommandBuffer() };

        flock.update(flockCommands);

		renderer.recordRenderCommands(flockCommands, ui, flock, image);

        flockCommands.addWaitSemaphore(freeImageSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        flockCommands.addSignalSemaphore(renderCompleteSemaphores[frame % QUEUE_SIZE].vk());

        context.device.submitCommandBuffer(flockCommands, fence.vk());

        // Queue rendered frame
        std::vector<VkSemaphore> swapChainWaitSemaphores{ renderCompleteSemaphores[frame % QUEUE_SIZE].vk() };
        swapChain.queueImage(imageIndex, swapChainWaitSemaphores);

		frame++;

        //context.device.graphicsQueueWaitIdle();
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

