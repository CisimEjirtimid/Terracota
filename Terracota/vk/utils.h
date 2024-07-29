#pragma once

// Vulkan init/include
#define VK_USE_PLATFORM_WIN32_KHR // Enable the WSI extensions
#define NOMINMAX
#include <vulkan/vulkan_raii.hpp>

namespace terracota::vk
{
    using namespace ::vk;

    using name = vk::ArrayWrapper1D<char, vk::MaxExtensionNameSize>;

    class name_vector
        : public std::vector<name>
    {
        std::vector<const char*> _raw;

    public:
        name_vector() = default;
        name_vector(const name_vector&) = default;
        name_vector(std::vector<const char*>& other);

        std::vector<const char*>& native();

        vk::name_vector intersect(vk::name_vector& other);
    };

    name_vector extensions();

    name_vector layers();
}