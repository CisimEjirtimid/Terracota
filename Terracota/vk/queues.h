#pragma once
#include "queue_infos.h"
#include "context.h"

namespace terracota::vk
{
    class queues
    {
        std::array<raii::Queue, queue_family::count> _queue_map;

    public:
        queues(context& context);

        raii::Queue& queue(const queue_family& family);
    };
}