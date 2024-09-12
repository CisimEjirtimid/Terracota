#include "utils.h"

namespace terracota::vk
{
    class viewport_info
    {
        vk::Viewport _viewport;
        vk::Rect2D _scissor;

    public:
        viewport_info(const vk::Extent2D& extent);

        vk::PipelineViewportStateCreateInfo& operator()() const;
    };
}