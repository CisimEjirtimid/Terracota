#pragma once
#include "utils.h"
#include "queue_info_index.h"

namespace terracota::vk
{
    struct device_create_info
    {
        // idx 0: graphics, idx 1: compute
        std::array<vk::DeviceQueueCreateInfo, 2> queue_infos;
        std::array<float, 2> queue_priorities{ 1.f, 1.f };

        vk::PhysicalDeviceFeatures physical_device_features;

        vk::DeviceCreateInfo device_info;

        device_create_info(vk::raii::PhysicalDevice& physical_device);
    };
}