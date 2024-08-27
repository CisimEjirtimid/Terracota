#pragma once

#include "instance_info.h"
#include "device_info.h"

#include "cen/utils.h"

namespace terracota::vk
{
    struct context
    {
        vk::raii::Context raii_context;
        vk::raii::Instance instance;
        vk::raii::SurfaceKHR surface;

        vk::raii::PhysicalDevice physical_device;

        vk::queue_infos qi;

        vk::device_info di;
        vk::raii::Device device;

        context(cen::window& window, const vk::instance_info& ii);
    };
}