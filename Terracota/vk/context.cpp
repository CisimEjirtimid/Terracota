#include "context.h"
#include "physical_device_selector.h"
#include <construct_array.h>

namespace terracota::vk
{
    namespace
    {
        // Create a Vulkan surface for rendering
        VkSurfaceKHR raw_surface(cen::window& window, raii::Instance& instance)
        {
            VkSurfaceKHR raw;
            cen::vk::make_surface(window, *instance, &raw);
            return raw;
        }
    }

    context::context(cen::window& window, const instance_info& ii, const physical_device_requirements& pdr)
        : instance{ raii_context, ii() }
        , surface{ instance, raw_surface(window, instance) }
        , physical_device{
            physical_device_selector{
                .surface = surface,
                .requirements = pdr
            }.pick(raii::PhysicalDevices{ instance }) }
        , qi{ physical_device, surface }
        , di{ qi, pdr.features() }
        , device{ physical_device, di() }
        , frames{ cisim::construct_array<frame_data, FRAME_OVERLAP>(device, qi) }
    {
    }
}
