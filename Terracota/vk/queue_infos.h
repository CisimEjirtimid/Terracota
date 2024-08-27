#pragma once
#include "utils.h"
#include "queue_family.h"

namespace terracota::vk
{
    class queue_infos
    {
        std::vector<DeviceQueueCreateInfo> _infos;

        // idx 0: graphics, idx 1: presentation, idx 2: compute
        std::array<uint32_t, queue_family::count> _family_indices;

    public:
        queue_infos(const raii::PhysicalDevice& physical_device, const raii::SurfaceKHR& surface);

        const std::vector<DeviceQueueCreateInfo>& operator()() const;

        uint32_t family_index(const queue_family& family) const;

        std::vector<uint32_t> family_indices() const;
    };
}