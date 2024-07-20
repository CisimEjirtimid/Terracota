#include "device_create_info.h"

namespace terracota::vk
{
    device_create_info::device_create_info(vk::raii::PhysicalDevice& physical_device)
    {
        using qi = queue_info_index;

        uint32_t current_idx = 0;

        for (const auto& qfp : physical_device.getQueueFamilyProperties())
        {
            if (qfp.queueFlags & vk::QueueFlagBits::eGraphics)
                queue_infos[qi::graphics].setQueueFamilyIndex(current_idx);

            if (qfp.queueFlags & vk::QueueFlagBits::eCompute)
                queue_infos[qi::compute].setQueueFamilyIndex(current_idx);

            current_idx++;
        }

        queue_infos[qi::graphics]
            .setQueueCount(1)
            .setPQueuePriorities(&queue_priorities[qi::graphics]);

        queue_infos[qi::compute]
            .setQueueCount(1)
            .setPQueuePriorities(&queue_priorities[qi::compute]);

        // TODO: specify physical device features later
        device_info = vk::DeviceCreateInfo()
            .setPEnabledFeatures(&physical_device_features)
            .setQueueCreateInfos(queue_infos);
    }
}