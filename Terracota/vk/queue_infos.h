#pragma once
#include "utils.h"
#include "queue_family.h"
#include <magic_enum_containers.hpp>

namespace terracota::vk
{
    class queue_infos
    {
        std::vector<DeviceQueueCreateInfo> _infos;

        // idx 0: graphics, idx 1: presentation, idx 2: compute
        std::array<uint32_t, queue_family::count> _family_indices;
        magic_enum::containers::bitset<queue_family> _family_set;

    public:
        queue_infos(const raii::PhysicalDevice& physical_device, const raii::SurfaceKHR& surface);

        const std::vector<DeviceQueueCreateInfo>& operator()() const;

        uint32_t family_index(const queue_family& family) const;

        std::vector<uint32_t> family_indices() const;

        magic_enum::containers::bitset<queue_family> family_set() const;
    };
}