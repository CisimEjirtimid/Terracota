#pragma once
#include "instance_info.h"
#include "device_info.h"
#include "frame_data.h"
#include "cen/utils.h"

namespace terracota::vk
{
    struct context
    {
        raii::Context raii_context;
        raii::Instance instance;
        raii::SurfaceKHR surface;

        raii::PhysicalDevice physical_device;

        queue_infos qi;

        device_info di;
        raii::Device device;

        std::array<frame_data, FRAME_OVERLAP> frames;

        context(cen::window& window, const instance_info& ii, const physical_device_requirements& pdr);
    };
}