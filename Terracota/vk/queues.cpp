#include "queues.h"
#include <bitset>

namespace terracota::vk
{
    queues::queues(context& context)
        : _queue_map
        {   // this order depends on queue_family order
            raii::Queue{ context.device, context.qi.family_index(queue_family::graphics), 0 },
            raii::Queue{ context.device, context.qi.family_index(queue_family::presentation), 0 },
            raii::Queue{ context.device, context.qi.family_index(queue_family::compute), 0 }
        }
    {}

    vk::raii::Queue& queues::queue(const queue_family& family)
    {
        return _queue_map[family];
    }
}