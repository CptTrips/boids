#include "VulkanContext.h"

const std::vector<const char*> VulkanContext::layers = {
#ifdef _DEBUG
    "VK_LAYER_KHRONOS_validation",
#endif
	"VK_LAYER_LUNARG_monitor"
};

const std::vector<const char*> VulkanContext::deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VulkanContext::VulkanContext()
    : window(600, 800, false)
	, instance(window.extensions(), layers)
	, surface(instance.vk(), window.window)
	, device(instance.vk(), surface.getVkSurface(), deviceExtensions, layers)
{

	//device.getPhysicalDeviceProperties();
}
