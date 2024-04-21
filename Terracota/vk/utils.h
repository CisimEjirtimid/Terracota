#pragma once

// Vulkan init/include
#define VK_USE_PLATFORM_WIN32_KHR // Enable the WSI extensions
#define NOMINMAX
#include <vulkan/vulkan_raii.hpp>

namespace terracota::vk
{
    using namespace ::vk;

    std::vector<ExtensionProperties> extensions();

    std::vector<LayerProperties> layers();
}