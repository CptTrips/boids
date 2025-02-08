#include "PushConstants.h"

const VkPushConstantRange PushConstants::range
{
	VK_SHADER_STAGE_COMPUTE_BIT,
	0,
	sizeof(PushConstants)
};