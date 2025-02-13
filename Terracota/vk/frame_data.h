#pragma once
#include "utils.h"
#include "queue_infos.h"

namespace terracota::vk
{
    constexpr size_t FRAME_OVERLAP = 2;

    struct frame_data
    {
        raii::CommandPool pool;
        raii::CommandBuffers buffers;

        // gpu -> gpu wait on swapchain image acquisition
        raii::Semaphore swapchain_semaphore;

        // gpu -> gpu wait on presenting the finished image
        raii::Semaphore present_semaphore;

        // gpu -> cpu wait on finishing current commands
        raii::Fence command_fence;

        frame_data(raii::Device& device, const queue_infos& qi);
    };
}