#include "context.h"
#include "cen/utils.h"

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
            return raw;
        }

        struct physical_device_requirements
        {
            magic_enum::containers::bitset<queue_family> queues;

            std::optional<vk::PhysicalDeviceType> device_type;
            std::optional<uint32_t> api_version;

            vk::PhysicalDeviceFeatures features10;
            vk::PhysicalDeviceVulkan11Features features11;
            vk::PhysicalDeviceVulkan12Features features12;
            vk::PhysicalDeviceVulkan13Features features13;
        };

        struct physical_device_selector
        {
            vk::raii::SurfaceKHR& surface;

            physical_device_requirements requirements;

            bool supports_features(concepts::physical_device_features auto requested, concepts::physical_device_features auto available)
            {
                auto requested_tuple = cisim::tuple::of_types<vk::Bool32>(requested.reflect());
                auto available_tuple = cisim::tuple::of_types<vk::Bool32>(available.reflect());

                return cisim::tuple::is_subset(requested_tuple, available_tuple);
            }

            bool appropriate(vk::raii::PhysicalDevice& physical_device)
            {
                queue_infos available_queues{ physical_device, surface };

                auto properties = physical_device.getProperties();
                auto features2 = physical_device.getFeatures2<
                    vk::PhysicalDeviceFeatures2,
                    vk::PhysicalDeviceVulkan11Features,
                    vk::PhysicalDeviceVulkan12Features,
                    vk::PhysicalDeviceVulkan13Features>();

                // check if required queues is subset of available queues
                if ((available_queues.family_set() & requirements.queues) != requirements.queues)
                    return false;

                if (requirements.api_version && requirements.api_version > properties.apiVersion)
                    return false;

                if (requirements.device_type && requirements.device_type != properties.deviceType)
                    return false;

                return supports_features(requirements.features10, features2.get<vk::PhysicalDeviceFeatures2>().features)
                    && supports_features(requirements.features11, features2.get<vk::PhysicalDeviceVulkan11Features>())
                    && supports_features(requirements.features12, features2.get<vk::PhysicalDeviceVulkan12Features>())
                    && supports_features(requirements.features13, features2.get<vk::PhysicalDeviceVulkan13Features>());
            }

            vk::raii::PhysicalDevice pick(vk::raii::PhysicalDevices physical_devices)
            {
                for (auto& physical_device : physical_devices)
                    if (appropriate(physical_device))
                        return physical_device;

                throw std::runtime_error{ "No appropriate physical device detected!" };
            }
        };
    }

    context::context(cen::window& window, const vk::instance_info& ii)
        : instance{ raii_context, ii() }
        , surface{ instance, raw_surface(window, instance) }
        , physical_device{
            physical_device_selector{
                .surface = surface,
                .requirements = physical_device_requirements{
                    .queues = {
                        queue_family::graphics,
                        queue_family::presentation
                    },
                    .device_type = vk::PhysicalDeviceType::eDiscreteGpu,
                    .api_version = ii().pApplicationInfo->apiVersion,
                    .features10 = vk::PhysicalDeviceFeatures{
                        .geometryShader = true,
                        .tessellationShader = true,
                        .shaderFloat64 = true,
                    }
                },
            }.pick(vk::raii::PhysicalDevices{ instance }) }
        , qi{ physical_device, surface }
        , di{ qi }
        , device{ physical_device, di() }
        // TODO: move to `vk::commands`
        , command_pool{ device, vk::CommandPoolCreateInfo{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = qi.family_index(vk::queue_family::graphics)
        } }
        , command_buffers{ device, vk::CommandBufferAllocateInfo{} }
    {
    }
}
