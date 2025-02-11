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

        frame_data(raii::Device& device, const queue_infos& qi);
    };
}