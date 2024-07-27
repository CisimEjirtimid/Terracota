#pragma once

namespace terracota::vk
{
    enum queue_info_index : uint8_t
    {
        graphics = 0,
        presentation,
        compute,

        // always leave this last
        count
    };
}