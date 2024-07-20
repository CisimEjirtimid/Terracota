#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct construct_params
    {
        vk::name_vector extensions;
        vk::name_vector layers;

        vk::InstanceCreateInfo instance_info;

        vk::PhysicalDeviceFeatures physical_device_features;
        vk::DeviceQueueCreateInfo queue_info;
        vk::DeviceCreateInfo device_info;

        construct_params(const vk::ApplicationInfo& application_info, std::optional<std::vector<const char*>> maybe_extensions = std::nullopt);
    };
}