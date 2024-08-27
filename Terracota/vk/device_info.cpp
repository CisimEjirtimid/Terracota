#include "device_info.h"
#include <bitset>

namespace terracota::vk
{
    device_info::device_info(vk::queue_infos& queue_infos)
        : _queue_infos{ queue_infos }
    {
        _required_physical_device_features
            .setTessellationShader(true)
            .setShaderFloat64(true)
            .setGeometryShader(true);

        _required_extensions.push_back(std::string_view{ vk::KHRSwapchainExtensionName });

        _info = vk::DeviceCreateInfo()
            .setPEnabledFeatures(&_required_physical_device_features)
            .setPEnabledExtensionNames(_required_extensions.native())
            .setQueueCreateInfos(_queue_infos());
    }

    const vk::DeviceCreateInfo& device_info::operator()() const
    {
        return _info;
    }
}