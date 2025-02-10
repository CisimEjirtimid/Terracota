#include "instance_info.h"

namespace terracota::vk
{
    instance_info::instance_info(const ApplicationInfo& application_info, std::optional<std::vector<const char*>> maybe_extensions)
    {
        // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)

        if (!maybe_extensions)
            throw std::runtime_error{ "No Vulkan extensions!" };

        name_vector required_extensions{ *maybe_extensions };
        name_vector available_extensions = extensions();

        _extensions = required_extensions.intersect(available_extensions);

        if (required_extensions.size() != _extensions.size())
            throw std::runtime_error{ "Required extensions not supported!" };

        // Use validation layers if this is a debug build
#if defined(_DEBUG)
        _layers.push_back(std::string_view{ "VK_LAYER_KHRONOS_validation" });
#endif

        // InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
        // are needed.
        _info = InstanceCreateInfo()
            .setPApplicationInfo(&application_info)
            .setPEnabledExtensionNames(_extensions.native())
            .setPEnabledLayerNames(_layers.native());
    }

    const InstanceCreateInfo& instance_info::operator()() const
    {
        return _info;
    }
}