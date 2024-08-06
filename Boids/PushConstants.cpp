#include "PushConstants.h"

const VkPushConstantRange PushConstants::range
{
	VK_SHADER_STAGE_COMPUTE_BIT,
	0,
	sizeof(uint32_t) + 3 * sizeof(float)
};