#include "Application.h"

#include "Vertex.h"

#include <chrono>
#include <thread>

void Application::processUIEvents()
{

}

Application::Application(ApplicationOptions options)
    : context()
	, ui()
	, renderer(RendererOptions(context, options.vertexShaderPath, options.fragmentShaderPath), options.flockSize)
	, flock(context.device, options.flockSize, options.computeShaderPath, options.initShaderPath)
{}

void Application::run()
{

    while (!context.window.windowShouldClose())
    {

		context.window.pollEvents();

        processUIEvents();

		{

			CommandBuffer flockCommands{ context.device.makeSingleUseCommandBuffer() };

			flock.update(flockCommands);

			context.device.submitCommandBuffer(flockCommands);

			context.device.graphicsQueueWaitIdle();
		}

		renderer.render(ui, flock);

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

