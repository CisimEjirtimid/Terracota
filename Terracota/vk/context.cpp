#include "context.h"
#include "cen/utils.h"

#include <execution>
#include "context.h"

namespace terracota::vk
{
    namespace
    {
        // Create a Vulkan surface for rendering
        VkSurfaceKHR raw_surface(cen::window& window, vk::raii::Instance& instance)
        {
            VkSurfaceKHR raw;
            cen::vk::make_surface(window, *instance, &raw);
            return std::move(raw);
        }

        vk::raii::PhysicalDevice pick_physical_device(vk::raii::Instance& instance)
        {
            auto valid = [](vk::raii::PhysicalDevice& physical_device)
                {
                    auto properties = physical_device.getProperties();
                    auto features = physical_device.getFeatures();

                    auto qfps = physical_device.getQueueFamilyProperties();

                    auto queue_flag_predicate = [](vk::QueueFlagBits flag_bit)
                        {
                            return [flag_bit](vk::QueueFamilyProperties& qfp) -> bool
                                {
                                    return bool(qfp.queueFlags & flag_bit);
                                };
                        };

                    using flag_predicate = std::invoke_result_t<decltype(queue_flag_predicate), vk::QueueFlagBits>;

                    auto exists_queue = [](std::vector<vk::QueueFamilyProperties>& qfps, flag_predicate&& predicate)
                        {
                            return std::find_if(std::execution::par, qfps.begin(), qfps.end(), predicate) != qfps.end();
                        };

                    auto graphics_queue = exists_queue(qfps, queue_flag_predicate(vk::QueueFlagBits::eGraphics));
                    auto compute_queue = exists_queue(qfps, queue_flag_predicate(vk::QueueFlagBits::eCompute));

                    // TODO: make these requirements configurable somehow
                    // predicate = std::function<bool(const vk::raii::PhysicalDevice&)> or
                    // predicate = std::function<bool(
                    //                  const vk::PhysicalDeviceProperties&,
                    //                  const vk::PhysicalDeviceFeatures&,
                    //                  const std::vector<QueueFamilyProperties>&)>
                    return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu
                        && features.shaderFloat64 // bools are uint32_t
                        && features.tessellationShader
                        && features.geometryShader
                        && graphics_queue && compute_queue; // these are proper bool
                };

            for (auto& physical_device : vk::raii::PhysicalDevices{ instance })
                if (valid(physical_device))
                    return physical_device;

            throw std::runtime_error{ "No valid physical device found!" };
        }
    }

    context::context(cen::window& window, const vk::instance_info& ii)
        : instance{ raii_context, ii() }
        , surface{ instance, raw_surface(window, instance) }
        , physical_device{ pick_physical_device(instance) }
        , qi{ physical_device, surface }
        , di{ qi }
        , device{ physical_device, di() }
    {
    }
}
