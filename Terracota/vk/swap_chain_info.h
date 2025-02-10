#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct swap_chain_info
    {
        SurfaceCapabilitiesKHR _surface_capabilities;

        std::vector<SurfaceFormatKHR> _surface_formats;
        std::vector<PresentModeKHR> _present_modes;

        std::vector<uint32_t> _queue_family_indices;

        SwapchainCreateInfoKHR _info;

    public:
        struct params
        {
            Extent2D framebuffer_size;
            std::vector<uint32_t> queue_family_indices;
        };

        swap_chain_info(raii::PhysicalDevice& physical_device, raii::SurfaceKHR& surface, const params& p);

        const SwapchainCreateInfoKHR& operator()() const;
    };
}