#include "Application.h"

#include "Vertex.h"

#include <chrono>
#include <thread>

Application::Application(ApplicationOptions options)
    : context()
	, renderer(RendererOptions(context, QUEUE_SIZE, options.vertexShaderPath, options.fragmentShaderPath), options.flockSize)
	, flock(context.device, { options.flockSize, 1.f, 1.f, 0.1f }, QUEUE_SIZE, options.computeShaderPath, options.initShaderPath)
	, ui({ &(flock.parameters.cohesion), &(flock.parameters.alignment), &(flock.parameters.separation) })
	, fences()
{

	for (uint32_t i{ 0 }; i < QUEUE_SIZE; i++)
		fences.emplace_back(context.device.vk());
}

void Application::run()
{

    while (!context.window.windowShouldClose())
    {

		Fence& fence{ fences[frame % QUEUE_SIZE ] };

		fence.wait(UINT64_MAX);

		fence.reset();

		context.window.pollEvents();

        CommandBuffer flockCommands{ context.device.makeSingleUseCommandBuffer() };

        flock.update(flockCommands);

		renderer.render(ui, flock, flockCommands);

        context.device.submitCommandBuffer(flockCommands, fence.vk());

		frame++;

        //context.device.graphicsQueueWaitIdle();
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

