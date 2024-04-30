#include "Renderer.h"

#include <vulkan/vulkan.h>

#include "CommandBuffer.h"
#include "Vertex.h"
#include "ShaderReader.h"


const std::vector<PipelineStage> Renderer::pipelineStages
{

	{VK_IMAGE_LAYOUT_UNDEFINED, 0, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT},
	{VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
	{VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT}
};

const std::vector<VkDescriptorSetLayoutBinding> Renderer::vertexDescriptorBindings
{

	{
		0,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		1,
		VK_SHADER_STAGE_VERTEX_BIT,
		nullptr
    }
};

const std::vector<VkDescriptorSetLayoutBinding> Renderer::fragmentDescriptorBindings
{
	{
		1,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		1,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		nullptr
    }
};

const std::vector<VkVertexInputBindingDescription> Renderer::vertexInputBindingDescriptions
{

	{ 0, sizeof(glm::vec2), VK_VERTEX_INPUT_RATE_VERTEX }
};

const std::vector<VkVertexInputAttributeDescription> Renderer::vertexAttributeDescriptions
{

	{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}
};

Renderer::Renderer(RendererOptions options)
	: device(options.device)
	, swapChain(device, options.surface, options.window)
	, pipelineBarriers(createPipelineBarriers())
	, uiRenderer({ options.window, device, options.instance, swapChain })
	, vertexDescriptorSetLayouts(makeVertexDescriptorSetLayouts())
	, fragmentDescriptorSetLayouts(makeFragmentDescriptorSetLayouts())
	, vertexShader(device, ShaderReader(options.vertexShaderPath).getCode(), VK_SHADER_STAGE_VERTEX_BIT, vertexDescriptorSetLayouts, vertexInputBindingDescriptions, vertexAttributeDescriptions, {})
	, fragmentShader(device, ShaderReader(options.fragmentShaderPath).getCode(), VK_SHADER_STAGE_FRAGMENT_BIT, fragmentDescriptorSetLayouts, {})
	, graphicsPipeline(device, swapChain.getFormat(), vertexShader, fragmentShader)
{

	/*
	*/
}

std::vector<PipelineBarrier> Renderer::createPipelineBarriers() const
{

	std::vector<PipelineBarrier> pipelineBarriers;

	PipelineStage prevStage{ pipelineStages[0] };

	for (auto it = pipelineStages.begin() + 1; it != pipelineStages.end(); it++)
	{

		pipelineBarriers.emplace_back(prevStage, *it);

		prevStage = *it;
	}

	return pipelineBarriers;
}

void Renderer::render(UI& ui, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer)
{

	// Acquire next available rendering resource pack (image, command buffer...)
	uint32_t imageIndex{ swapChain.getFreeImageIndex() };
	Image image(swapChain.getImage(imageIndex));

	// Record rendering instructions in command buffer
	CommandBuffer commandBuffer(device.makeSingleUseCommandBuffer());

	recordRenderCommands(commandBuffer, image, ui, vertexBuffer, indexBuffer);

	// Submit command buffer
	device.submitCommandBuffer(commandBuffer);

	// Queue rendered frame
	swapChain.queueImage(imageIndex);

	device.graphicsQueueWaitIdle();
}

std::vector<DescriptorSetLayout> Renderer::makeVertexDescriptorSetLayouts() const
{

	std::vector<DescriptorSetLayout> layouts;

	layouts.emplace_back(device, vertexDescriptorBindings);

	return layouts;
}

std::vector<DescriptorSetLayout> Renderer::makeFragmentDescriptorSetLayouts() const
{

	std::vector<DescriptorSetLayout> layouts;

	layouts.emplace_back(device, fragmentDescriptorBindings);

	return layouts;
}

void Renderer::recordRenderCommands(CommandBuffer& commandBuffer, Image & image, UI& ui, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer)
{

	pipelineBarriers[0].layoutTransition(commandBuffer, image);

	beginRendering(commandBuffer, image);

	uiRenderer.render(commandBuffer, ui);

	bindObjects(commandBuffer, vertexBuffer, indexBuffer);

	drawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer.size() / sizeof(uint32_t)));

	vkCmdEndRendering(commandBuffer.vk());

	pipelineBarriers[1].layoutTransition(commandBuffer, image);
}

void Renderer::beginRendering(CommandBuffer & commandBuffer, const Image & image) const
{

	VkRenderingAttachmentInfo colourAttachmentInfo{};
	
	colourAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colourAttachmentInfo.imageView = image.getImageView();
	colourAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colourAttachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
	colourAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colourAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colourAttachmentInfo.clearValue = { 0.0f, 0.0f, 0.0f, 1.0f };

	VkRenderingInfo renderInfo{};

	renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderInfo.renderArea.offset = { 0, 0 };
	renderInfo.renderArea.extent = swapChain.getExtent();
	renderInfo.layerCount = 1;
	renderInfo.viewMask = 0;
	renderInfo.colorAttachmentCount = 1;
	renderInfo.pColorAttachments = &colourAttachmentInfo;
	renderInfo.pDepthAttachment = VK_NULL_HANDLE;
	renderInfo.pStencilAttachment = VK_NULL_HANDLE;
	
	vkCmdBeginRendering(commandBuffer.vk(), &renderInfo);

}

void Renderer::bindObjects(CommandBuffer& commandBuffer, DeviceBuffer& vertexBuffer, DeviceBuffer& indexBuffer) const
{

	graphicsPipeline.bind(commandBuffer);

    VkBuffer vertexBuffers[] = { vertexBuffer.vk() };

	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer.vk(), 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer.vk(), indexBuffer.vk(), 0, VK_INDEX_TYPE_UINT32);

	VkExtent2D swapChainExtent{ swapChain.getExtent() };

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChainExtent.width;
	viewport.height = (float) swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer.vk(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer.vk(), 0, 1, &scissor);

}

void Renderer::drawIndexed(CommandBuffer& commandBuffer, uint32_t indexCount) const
{

	int instanceCount = 1;
	int firstIndex = 0;
	int vertexOffset = 0;
	int firstInstance = 0;
	vkCmdDrawIndexed(commandBuffer.vk(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
