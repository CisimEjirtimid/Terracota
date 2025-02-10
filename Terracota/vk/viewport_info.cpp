#include "viewport_info.h"

namespace terracota::vk
{
    viewport_info::viewport_info(const Extent2D& extent)
        : _viewport{ 0.f, 0.f, float(extent.width), float(extent.height), 0.f, 1.f }
        , _scissor{ { 0, 0 }, extent }
    {}

    PipelineViewportStateCreateInfo& viewport_info::operator()() const
    {
        static auto info = PipelineViewportStateCreateInfo{}
            .setScissors(_scissor)      // Careful with static in function scope
            .setViewports(_viewport);   // it gets created only on first function call

        return info;
    }
}