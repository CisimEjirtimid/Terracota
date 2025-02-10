#include "device_info.h"
#include <bitset>

namespace terracota::vk
{
    device_info::device_info(const queue_infos& queue_infos, const required_features& features)
        : _extensions{ KHRSwapchainExtensionName }
        , _info{
            DeviceCreateInfo() // left setters because of the array proxy init
                .setPEnabledFeatures(&features.get<PhysicalDeviceFeatures2>().features)
                .setPEnabledExtensionNames(_extensions.native())
                .setQueueCreateInfos(queue_infos()),
            features.get<PhysicalDeviceVulkan11Features>(),
            features.get<PhysicalDeviceVulkan12Features>(),
            features.get<PhysicalDeviceVulkan13Features>()
        }
    {
    }

    const DeviceCreateInfo& device_info::operator()() const
    {
        return _info.get();
    }
}