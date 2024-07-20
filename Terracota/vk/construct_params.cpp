#include "construct_params.h"
#include "strcmp_functor.h"

namespace terracota::vk
{
    construct_params::construct_params(const vk::ApplicationInfo& application_info, std::optional<std::vector<const char*>> maybe_extensions)
    {
        // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)

        if (!maybe_extensions)
            throw std::runtime_error{ "No Vulkan extensions!" };

        vk::name_vector required_extensions{ *maybe_extensions };
        std::sort(required_extensions.begin(), required_extensions.end(), cisim::strcmp_functor{});

        auto available_extensions = vk::extensions();
        std::sort(available_extensions.begin(), available_extensions.end(), cisim::strcmp_functor{});

        std::set_intersection(
            required_extensions.begin(), required_extensions.end(),
            available_extensions.begin(), available_extensions.end(),
            std::back_inserter(extensions), cisim::strcmp_functor{});

        if (required_extensions.size() != extensions.size())
            throw std::runtime_error{ "Required extensions not supported!" };

        // Use validation layers if this is a debug build
#if defined(_DEBUG)
        layers.push_back(std::string_view{ "VK_LAYER_KHRONOS_validation" });
#endif

        // vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
        // are needed.
        instance_info = vk::InstanceCreateInfo()
            .setPApplicationInfo(&application_info)
            .setPEnabledExtensionNames(extensions.native())
            .setPEnabledLayerNames(layers.native());
    }
}