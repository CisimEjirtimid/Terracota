#include "frame_data.h"

namespace terracota::vk
{
    frame_data::frame_data(raii::Device& device, const queue_infos& qi)
        : pool{
            device, CommandPoolCreateInfo
            {
                .flags = CommandPoolCreateFlagBits::eResetCommandBuffer,
                .queueFamilyIndex = qi.family_index(queue_family::graphics)
            }
        }
        , buffers{ device, CommandBufferAllocateInfo
            {
                .commandPool = pool,
                .level = CommandBufferLevel::ePrimary,
                .commandBufferCount = 1,
            }
        }
        , swapchain_semaphore{ device, SemaphoreCreateInfo{} }
        , present_semaphore{ device, SemaphoreCreateInfo{} }
        , command_fence{ device, FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled } }
    {
    }
}