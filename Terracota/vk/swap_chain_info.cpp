#include "swap_chain_info.h"

namespace terracota::vk
{
    namespace
    {
        uint32_t min_image_count(SurfaceCapabilitiesKHR& sc)
        {
            uint32_t count = sc.minImageCount + 1; // at least one more image in swap chain

            // 0 is special value that states that there is no maximum image count on a system
            if (sc.maxImageCount > 0 && count > sc.maxImageCount)
                count = sc.maxImageCount;

            return count;
        }

        SurfaceFormatKHR pick_surface_format(const std::vector<SurfaceFormatKHR>& surface_formats)
        {
            if (surface_formats.empty())
                throw std::runtime_error{ "No Surface Formats detected!" };

            for (auto& f : surface_formats)
                if (f.format == Format::eB8G8R8A8Srgb &&
                    f.colorSpace == ColorSpaceKHR::eSrgbNonlinear)
                    return f;

            // return first if non was suitable
            return surface_formats[0];
        }

        PresentModeKHR pick_present_mode(const std::vector<PresentModeKHR>& present_modes)
        {
            for (auto& m : present_modes)
                if (m == PresentModeKHR::eMailbox)
                    return m;

            // FIFO present mode (vsync) is guaranteed
            return PresentModeKHR::eFifo;
        }

        Extent2D pick_extent(SurfaceCapabilitiesKHR& sc, const Extent2D& size)
        {
            if (valid(sc.currentExtent))
                return sc.currentExtent;

            return Extent2D{
                std::clamp(size.width, sc.minImageExtent.width, sc.maxImageExtent.width),
                std::clamp(size.height, sc.minImageExtent.height, sc.maxImageExtent.height),
            };
        }
    }

    swap_chain_info::swap_chain_info(
        raii::PhysicalDevice& physical_device,
        raii::SurfaceKHR& surface,
        const params& p)
        : _surface_capabilities(physical_device.getSurfaceCapabilitiesKHR(surface))
        , _surface_formats(physical_device.getSurfaceFormatsKHR(surface))
        , _present_modes(physical_device.getSurfacePresentModesKHR(surface))
        , _queue_family_indices(p.queue_family_indices)
    {
        auto surface_format = pick_surface_format(_surface_formats);

        _info
            .setSurface(surface)
            .setMinImageCount(min_image_count(_surface_capabilities))
            .setImageFormat(surface_format.format)
            .setImageColorSpace(surface_format.colorSpace)
            .setImageExtent(pick_extent(_surface_capabilities, p.framebuffer_size))
            .setImageArrayLayers(1)
            .setImageUsage(ImageUsageFlagBits::eColorAttachment)
            .setQueueFamilyIndices(_queue_family_indices)
            .setImageSharingMode(
                _queue_family_indices.size() != 1
                ? SharingMode::eConcurrent
                : SharingMode::eExclusive);
    }

    const SwapchainCreateInfoKHR& swap_chain_info::operator()() const
    {
        return _info;
    }
}
