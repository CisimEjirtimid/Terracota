#include "swap_chain_info.h"

namespace terracota::vk
{
    namespace
    {
        vk::SurfaceFormatKHR pick_surface_format(vk::raii::PhysicalDevice& physical_device)
        {
        }

        vk::PresentModeKHR pick_present_mode(vk::raii::PhysicalDevice& physical_device)
        {
        }

        vk::Extent2D pick_extent(vk::raii::SurfaceKHR& surface)
        {
        }
    }
    swap_chain_info::swap_chain_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface)
        : surface_capabilities(physical_device.getSurfaceCapabilitiesKHR(surface))
        , surface_formats(physical_device.getSurfaceFormatsKHR(surface))
        , present_modes(physical_device.getSurfacePresentModesKHR(surface))
    {
        // TODO: fill up info struct
        // info = vk::SwapchainCreateInfoKHR()
    }
}
