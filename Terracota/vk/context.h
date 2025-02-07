#pragma once
#include <tuple>
#include <concepts>
#include "instance_info.h"
#include "device_info.h"

#include "cen/utils.h"

namespace terracota::vk
{
    namespace concepts
    {
        template <typename T>
        concept physical_device_features =
            std::same_as<T, vk::PhysicalDeviceFeatures> ||
            std::same_as<T, vk::PhysicalDeviceVulkan11Features> ||
            std::same_as<T, vk::PhysicalDeviceVulkan12Features> ||
            std::same_as<T, vk::PhysicalDeviceVulkan13Features>;
    }

    struct context
    {
        vk::raii::Context raii_context;
        vk::raii::Instance instance;
        vk::raii::SurfaceKHR surface;

        vk::raii::PhysicalDevice physical_device;

        vk::queue_infos qi;

        vk::device_info di;
        vk::raii::Device device;

        vk::raii::CommandPool command_pool;
        vk::raii::CommandBuffers command_buffers;

        context(cen::window& window, const vk::instance_info& ii);
    };
}