#pragma once
#include "utils.h"

namespace terracota::vk
{
    struct instance_info
    {
        vk::name_vector extensions;
        vk::name_vector layers;

        vk::InstanceCreateInfo info;

        instance_info(const vk::ApplicationInfo& application_info, std::optional<std::vector<const char*>> maybe_extensions = std::nullopt);
    };
}