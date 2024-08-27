#pragma once
#include "utils.h"
#include "queue_infos.h"

namespace terracota::vk
{
    struct device_info
    {
        vk::queue_infos& _queue_infos;

        vk::PhysicalDeviceFeatures _required_physical_device_features;
        vk::name_vector _required_extensions;

        vk::DeviceCreateInfo _info;

    public:
        device_info(vk::queue_infos& queue_infos);

        const vk::DeviceCreateInfo& operator()() const;
    };
}