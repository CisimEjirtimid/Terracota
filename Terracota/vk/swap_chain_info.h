#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct swap_chain_info
    {
        vk::SurfaceCapabilitiesKHR surface_capabilities;

        std::vector<vk::SurfaceFormatKHR> surface_formats;
        std::vector<vk::PresentModeKHR> present_modes;

        vk::SwapchainCreateInfoKHR info;

        swap_chain_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface, const vk::Extent2D& window_size);
    };
}