#pragma once
#include "utils.h"
#include "queue_infos.h"
#include "physical_device_selector.h"

namespace terracota::vk
{
    struct device_info
    {
        // Device extensions TODO: parametrize?
        name_vector _extensions;

        StructureChain<
            DeviceCreateInfo,
            PhysicalDeviceVulkan11Features,
            PhysicalDeviceVulkan12Features,
            PhysicalDeviceVulkan13Features> _info;

    public:
        device_info(const queue_infos& queue_infos, const required_features& features);

        const DeviceCreateInfo& operator()() const;
    };
}