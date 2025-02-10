#pragma once
#include <magic_enum_containers.hpp>
#include "tuple.h"
#include "utils.h"
#include "queue_family.h"

namespace terracota::vk
{
    namespace concepts
    {
        template <typename T>
        concept physical_device_features =
            std::same_as<T, PhysicalDeviceFeatures> ||
            std::same_as<T, PhysicalDeviceVulkan11Features> ||
            std::same_as<T, PhysicalDeviceVulkan12Features> ||
            std::same_as<T, PhysicalDeviceVulkan13Features>;
    }

    using required_features = StructureChain<
        PhysicalDeviceFeatures2,
        PhysicalDeviceVulkan11Features,
        PhysicalDeviceVulkan12Features,
        PhysicalDeviceVulkan13Features>;

    struct physical_device_requirements
    {
        magic_enum::containers::bitset<queue_family> queues;

        std::optional<PhysicalDeviceType> device_type;
        std::optional<uint32_t> api_version;

        PhysicalDeviceFeatures features10;
        PhysicalDeviceVulkan11Features features11;
        PhysicalDeviceVulkan12Features features12;
        PhysicalDeviceVulkan13Features features13;

        required_features features() const;
    };

    struct physical_device_selector
    {
        raii::SurfaceKHR& surface;

        physical_device_requirements requirements;

        bool supports_features(concepts::physical_device_features auto requested, concepts::physical_device_features auto available);

        bool appropriate(raii::PhysicalDevice& physical_device);

        raii::PhysicalDevice pick(raii::PhysicalDevices physical_devices);
    };
}