#include "utils.h"

namespace terracota::vk
{
    class viewport_info
    {
        Viewport _viewport;
        Rect2D _scissor;

    public:
        viewport_info(const Extent2D& extent);

        PipelineViewportStateCreateInfo& operator()() const;
    };
}