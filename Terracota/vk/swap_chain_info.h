#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct swap_chain_info
    {
        vk::SurfaceCapabilitiesKHR surface_capabilities;

        std::vector<vk::SurfaceFormatKHR> surface_formats;
        std::vector<vk::PresentModeKHR> present_modes;

        std::vector<uint32_t> queue_family_indices;

        vk::SwapchainCreateInfoKHR info;

        struct params
        {
            vk::Extent2D framebuffer_size;
            std::vector<uint32_t> queue_family_indices;
        };

        swap_chain_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface, const params& p);
    };
}