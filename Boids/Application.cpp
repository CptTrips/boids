#include "Application.h"

#include "Vertex.h"

#include <chrono>
#include <thread>

void Application::processUIEvents(CommandBuffer& commandBuffer)
{

	UIState state{ ui.getState() };

	flock.setCohesion(commandBuffer, state.cohesion);

	flock.setSeparation(commandBuffer, state.separation);

	flock.setAlignment(commandBuffer, state.alignment);
}

Application::Application(ApplicationOptions options)
    : context()
	, ui({ 2.f, 2.f, 1.f })
	, renderer(RendererOptions(context, options.vertexShaderPath, options.fragmentShaderPath), options.flockSize)
	, flock(context.device, options.flockSize, options.computeShaderPath, options.initShaderPath)
{}

void Application::run()
{

    while (!context.window.windowShouldClose())
    {

		context.window.pollEvents();

		{

			CommandBuffer flockCommands{ context.device.makeSingleUseCommandBuffer() };

            processUIEvents(flockCommands);

			flock.update(flockCommands);

			context.device.submitCommandBuffer(flockCommands);

			context.device.graphicsQueueWaitIdle();
		}

		renderer.render(ui, flock);

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

