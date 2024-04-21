#include "utils.h"

namespace terracota::vk
{
    std::vector<ExtensionProperties> extensions()
    {
        return enumerateInstanceExtensionProperties();
    }

    std::vector<LayerProperties> layers()
    {
        return enumerateInstanceLayerProperties();
    }
}
