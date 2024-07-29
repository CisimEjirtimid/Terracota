#include "device_info.h"
#include <bitset>

namespace terracota::vk
{
    device_info::device_info(vk::raii::PhysicalDevice& physical_device, vk::raii::SurfaceKHR& surface)
    {
        using qi = queue_info_index;

        uint32_t current_idx = 0;

        std::bitset<qi::count> set;

        for (const auto& qfp : physical_device.getQueueFamilyProperties())
        {
            bool selected = false;

            if (qfp.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                queue_family_indices[qi::graphics] = current_idx;
                set[qi::graphics] = true;

                selected = true;
            }

            if (physical_device.getSurfaceSupportKHR(current_idx, surface))
            {
                queue_family_indices[qi::presentation] = current_idx;
                set[qi::presentation] = true;

                selected = true;
            }

            if (qfp.queueFlags & vk::QueueFlagBits::eCompute)
            {
                queue_family_indices[qi::compute] = current_idx;
                set[qi::compute] = true;

                selected = true;
            }

            if (selected)
            {
                queue_priorities.push_back(1.f);

                queue_infos.push_back(
                    vk::DeviceQueueCreateInfo{}
                        .setQueueFamilyIndex(current_idx)
                        .setQueueCount(1)
                        .setPQueuePriorities(&queue_priorities.back()));
            }

            if (set.all())
                break;

            current_idx++;
        }

        required_physical_device_features
            .setTessellationShader(true)
            .setShaderFloat64(true)
            .setGeometryShader(true);

        required_extensions.push_back(std::string_view{ vk::KHRSwapchainExtensionName });

        info = vk::DeviceCreateInfo()
            .setPEnabledFeatures(&required_physical_device_features)
            .setPEnabledExtensionNames(required_extensions.native())
            .setQueueCreateInfos(queue_infos);
    }
}