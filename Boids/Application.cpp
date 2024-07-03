#include "Application.h"

#include "Vertex.h"

Application::Application(ApplicationOptions options)
    : context()
	, swapChain(context.device, context.surface, context.window, QUEUE_SIZE + 2)
	, renderer(RendererOptions(context, QUEUE_SIZE, options.vertexShaderPath, options.fragmentShaderPath, swapChain), options.flockSize)
	, flock(context.device, { options.flockSize, cohesion, alignment, separation}, QUEUE_SIZE, options.computeShaderPath, options.initShaderPath)
	, ui({ &(flock.parameters.cohesion), &(flock.parameters.alignment), &(flock.parameters.separation) })
	, fences()
	, freeImageSemaphores()
	, renderCompleteSemaphores()
	, commandBuffers(context.device.makeCommandBuffers(QUEUE_SIZE, false))
	, computeTimer(context.device, QUEUE_SIZE)
	, drawTimer(context.device, QUEUE_SIZE)
	, frameTimer(context.device, QUEUE_SIZE)
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

		if (frame > QUEUE_SIZE)
		{

            float frameTime{ frameTimer.getTime(index) / 1e6f };

			float computeTime{ computeTimer.getTime(index) / 1e6f };

			float drawTime{ drawTimer.getTime(index) / 1e6f };

            //printf("Frame : %5.1fms Compute : %5.1fms Draw : %5.1fms\n", frametime, computeTime, drawTime);

			ui.setFrametime(frameTime, computeTime, drawTime);

			perfLog.log(computeTime, drawTime, frameTime);
		}

		frameTimer.startTimer(flockCommands, index, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

		computeTimer.startTimer(flockCommands, index, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

        flock.update(flockCommands);

		computeTimer.stopTimer(flockCommands, index, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

		drawTimer.startTimer(flockCommands, index, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

		renderer.recordRenderCommands(flockCommands, ui, flock, image);

		drawTimer.stopTimer(flockCommands, index, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		frameTimer.stopTimer(flockCommands, index, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        flockCommands.addWaitSemaphore(freeImageSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        flockCommands.addSignalSemaphore(renderCompleteSemaphores[index].vk());

        context.device.submitCommandBuffer(flockCommands, fence.vk());

        // Queue rendered frame
        std::vector<VkSemaphore> swapChainWaitSemaphores{ renderCompleteSemaphores[index].vk() };
        swapChain.queueImage(imageIndex, swapChainWaitSemaphores);

		frame++;
    }

	perfLog.print();

	context.device.graphicsQueueWaitIdle();
}

void PerformanceLog::log(float computeTime, float drawTime, float frameTime)
{

    rollingAppend(computeTimes, computeTime);
    rollingAppend(drawTimes, drawTime);
    rollingAppend(frameTimes, frameTime);

}

void PerformanceLog::print() const
{

	printf("compute,draw,frame\n");

	for (size_t i{ 0 }; i < computeTimes.size(); i++)
		printf("%.2f,%.2f,%.2f\n", computeTimes[i], drawTimes[i], frameTimes[i]);
}

void PerformanceLog::rollingAppend(std::vector<float>& v, float x)
{
	if (v.size() > 1024)
		v.clear();

	v.push_back(x);
}
