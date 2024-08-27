#include "context.h"

namespace terracota::cen
{
    context::context(const std::string& title, const cen::iarea& size)
        : sdl{ cen::sdl_cfg{ SDL_INIT_VIDEO } }
        , window{
            title.c_str(),
            size,
            cen::window::window_flags::vulkan
        }
    {
    }
}