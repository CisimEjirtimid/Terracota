#pragma once
#include "utils.h"

namespace terracota::vk
{
    class instance_info
    {
        vk::name_vector _extensions;
        vk::name_vector _layers;

        vk::InstanceCreateInfo _info;

    public:
        instance_info(const vk::ApplicationInfo& application_info, std::optional<std::vector<const char*>> maybe_extensions = std::nullopt);

        const vk::InstanceCreateInfo& operator()() const;
    };
}