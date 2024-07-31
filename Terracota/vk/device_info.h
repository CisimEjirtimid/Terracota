#pragma once
#include "utils.h"
#include "queue_info_index.h"

namespace terracota::vk
{
    struct device_info
    {
        std::vector<vk::DeviceQueueCreateInfo> queue_infos;
        std::vector<float> queue_priorities;

        // idx 0: graphics, idx 1: presentation, idx 2: compute
        std::array<uint32_t, queue_info_index::count> queue_family_index;

        vk::PhysicalDeviceFeatures required_physical_device_features;
        vk::name_vector required_extensions;

        vk::DeviceCreateInfo info;

        device_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface);

        std::vector<uint32_t> queue_family_indices() const;
    };
}