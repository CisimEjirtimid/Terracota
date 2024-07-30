#include "swap_chain_info.h"

namespace terracota::vk
{
    namespace
    {
        uint32_t min_image_count(vk::SurfaceCapabilitiesKHR& sc)
        {
            uint32_t count = sc.minImageCount + 1; // at least one more image in swap chain

            // 0 is special value that states that there is no maximum image count on a system
            if (sc.maxImageCount > 0 && count > sc.maxImageCount)
                count = sc.maxImageCount;

            return count;
        }

        vk::SurfaceFormatKHR pick_surface_format(const std::vector<vk::SurfaceFormatKHR>& surface_formats)
        {
            if (surface_formats.empty())
                throw std::runtime_error{ "No Surface Formats detected!" };

            for (auto& f : surface_formats)
                if (f.format == vk::Format::eB8G8R8A8Srgb &&
                    f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                    return f;

            // return first if non was suitable
            return surface_formats[0];
        }

        vk::PresentModeKHR pick_present_mode(const std::vector<vk::PresentModeKHR>& present_modes)
        {
            for (auto& m : present_modes)
                if (m == vk::PresentModeKHR::eMailbox)
                    return m;

            // FIFO present mode (vsync) is guaranteed
            return vk::PresentModeKHR::eFifo;
        }

        vk::Extent2D pick_extent(vk::SurfaceCapabilitiesKHR& sc, const vk::Extent2D& size)
        {
            if (valid(sc.currentExtent))
                return sc.currentExtent;

            return vk::Extent2D{
                std::clamp(size.width, sc.minImageExtent.width, sc.maxImageExtent.width),
                std::clamp(size.height, sc.minImageExtent.height, sc.maxImageExtent.height),
            };
        }
    }

    swap_chain_info::swap_chain_info(
        vk::raii::PhysicalDevice& physical_device,
        vk::raii::SurfaceKHR& surface,
        const vk::Extent2D& window_size)
        : surface_capabilities(physical_device.getSurfaceCapabilitiesKHR(surface))
        , surface_formats(physical_device.getSurfaceFormatsKHR(surface))
        , present_modes(physical_device.getSurfacePresentModesKHR(surface))
    {
        auto surface_format = pick_surface_format(surface_formats);

        info
            .setSurface(surface)
            .setMinImageCount(min_image_count(surface_capabilities))
            .setImageFormat(surface_format.format)
            .setImageColorSpace(surface_format.colorSpace)
            .setImageExtent(pick_extent(surface_capabilities, window_size))
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            //.setImageSharingMode() TODO: based on number of queue families
            //.setQueueFamilyIndices()
            ;
    }
}
