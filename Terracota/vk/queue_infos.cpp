#include "queue_infos.h"
#include <bitset>

namespace terracota::vk
{
    namespace
    {
        static constexpr float default_priority = 1.f;
    }

    queue_infos::queue_infos(const vk::raii::PhysicalDevice& physical_device, const vk::raii::SurfaceKHR& surface)
    {
        using qf = queue_family;

        uint32_t current_idx = 0;

        std::bitset<qf::count> set;

        for (const auto& qfp : physical_device.getQueueFamilyProperties())
        {
            bool selected = false;

            if (qfp.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                _family_indices[qf::graphics] = current_idx;
                set[qf::graphics] = true;

                selected = true;
            }

            if (physical_device.getSurfaceSupportKHR(current_idx, surface))
            {
                _family_indices[qf::presentation] = current_idx;
                set[qf::presentation] = true;

                selected = true;
            }

            if (qfp.queueFlags & vk::QueueFlagBits::eCompute)
            {
                _family_indices[qf::compute] = current_idx;
                set[qf::compute] = true;

                selected = true;
            }

            if (selected)
                _infos.push_back(
                    vk::DeviceQueueCreateInfo{}
                    .setQueueFamilyIndex(current_idx)
                    .setQueueCount(1)
                    .setPQueuePriorities(&default_priority));

            if (set.all())
                break;

            current_idx++;
        }
    }

    const std::vector<vk::DeviceQueueCreateInfo>& queue_infos::operator()() const
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
}