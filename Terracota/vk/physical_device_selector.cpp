#include "physical_device_selector.h"
#include "queue_infos.h"

namespace terracota::vk
{
    required_features physical_device_requirements::features() const
    {
        return {
            PhysicalDeviceFeatures2{
                .features = features10
            },
            features11,
            features12,
            features13
        };
    }

    bool physical_device_selector::supports_features(concepts::physical_device_features auto requested, concepts::physical_device_features auto available)
    {
        auto requested_tuple = cisim::tuple::of_types<Bool32>(requested.reflect());
        auto available_tuple = cisim::tuple::of_types<Bool32>(available.reflect());

        return cisim::tuple::is_subset(requested_tuple, available_tuple);
    }

    bool physical_device_selector::appropriate(raii::PhysicalDevice& physical_device)
    {
        queue_infos available_queues{ physical_device, surface };

        auto properties = physical_device.getProperties();
        auto features2 = physical_device.getFeatures2<
            PhysicalDeviceFeatures2,
            PhysicalDeviceVulkan11Features,
            PhysicalDeviceVulkan12Features,
            PhysicalDeviceVulkan13Features>();

        // check if required queues is subset of available queues
        if ((available_queues.family_set() & requirements.queues) != requirements.queues)
            return false;

        if (requirements.api_version && requirements.api_version > properties.apiVersion)
            return false;

        if (requirements.device_type && requirements.device_type != properties.deviceType)
            return false;

        return supports_features(requirements.features10, features2.get<PhysicalDeviceFeatures2>().features)
            && supports_features(requirements.features11, features2.get<PhysicalDeviceVulkan11Features>())
            && supports_features(requirements.features12, features2.get<PhysicalDeviceVulkan12Features>())
            && supports_features(requirements.features13, features2.get<PhysicalDeviceVulkan13Features>());
    }

    raii::PhysicalDevice physical_device_selector::pick(raii::PhysicalDevices physical_devices)
    {
        for (auto& physical_device : physical_devices)
            if (appropriate(physical_device))
                return physical_device;

        throw std::runtime_error{ "No appropriate physical device detected!" };
    }
}