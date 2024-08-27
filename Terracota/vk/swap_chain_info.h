#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct swap_chain_info
    {
        vk::SurfaceCapabilitiesKHR _surface_capabilities;

        std::vector<vk::SurfaceFormatKHR> _surface_formats;
        std::vector<vk::PresentModeKHR> _present_modes;

        std::vector<uint32_t> _queue_family_indices;

        vk::SwapchainCreateInfoKHR _info;

    public:
        struct params
        {
            vk::Extent2D framebuffer_size;
            std::vector<uint32_t> queue_family_indices;
        };

        swap_chain_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface, const params& p);

        const vk::SwapchainCreateInfoKHR& operator()() const;
    };
}