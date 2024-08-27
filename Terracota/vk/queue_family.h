#pragma once
#include <cstdint>

namespace terracota::vk
{
    enum queue_family : uint8_t
    {
        graphics = 0,
        presentation,
        compute,

        // always leave this last
        count
    };
}