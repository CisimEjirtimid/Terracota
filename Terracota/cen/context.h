#pragma once
#include "utils.h"

namespace terracota::cen
{
    using namespace ::cen;

    struct context
    {
        cen::sdl sdl;
        cen::window window;

        context(const std::string& title, const cen::iarea& size);
    };
}