#include "queue_infos.h"
#include <execution>

namespace terracota::vk
{
    namespace
    {
        static constexpr float default_priority = 1.f;
    }

    queue_infos::queue_infos(const raii::PhysicalDevice& physical_device, const raii::SurfaceKHR& surface)
    {
        using qf = queue_family;

        uint32_t current_idx = 0;

        for (const auto& qfp : physical_device.getQueueFamilyProperties())
        {
            bool selected = false;

            auto set = [this, &selected, &current_idx](auto queue_family)
                {
                    _family_indices[queue_family] = current_idx;
                    _family_set[queue_family] = true;

                    selected = true;
                };

            if (qfp.queueFlags & QueueFlagBits::eGraphics)
                set(queue_family::graphics);

            if (physical_device.getSurfaceSupportKHR(current_idx, surface))
                set(queue_family::presentation);

            if (qfp.queueFlags & QueueFlagBits::eCompute)
                set(qf::compute);

            if (selected)
                _infos.push_back(
                    DeviceQueueCreateInfo{
                        .queueFamilyIndex = current_idx,
                        .queueCount = 1,
                        .pQueuePriorities = &default_priority
                    });

            if (_family_set.all())
                break;

            current_idx++;
        }
    }

    const std::vector<DeviceQueueCreateInfo>& queue_infos::operator()() const
    {
        return _infos;
    }

    uint32_t queue_infos::family_index(const queue_family& family) const
    {
        return _family_indices[family];
    }

    std::vector<uint32_t> queue_infos::family_indices() const
    {
        std::vector<uint32_t> result;

        for (auto& info : _infos)
            result.push_back(info.queueFamilyIndex);

        return result;
    }

    magic_enum::containers::bitset<queue_family> queue_infos::family_set() const
    {
        return _family_set;
    }
}