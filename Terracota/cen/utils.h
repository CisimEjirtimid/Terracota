#pragma once

// SDL/Centurion init/include
#define SDL_MAIN_HANDLED
#define CENTURION_NO_SDL_IMAGE
#define CENTURION_NO_SDL_MIXER
#define CENTURION_NO_SDL_TTF
#define CENTURION_NO_OPENGL

// deprecation warning suppression
#pragma warning(push)
#pragma warning(disable:4996)
#include <centurion/centurion.hpp>
#pragma warning(pop)

namespace terracota::cen
{
    using namespace ::cen;

    using u32area = basic_area<uint32_t>;
    using i32area = basic_area<int32_t>;
    using sarea = basic_area<size_t>;

    template <typename T, typename U>
        requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
    basic_area<T> cast(const basic_area<U>& value)
    {
        return basic_area<T>{ static_cast<T>(value.width), static_cast<T>(value.height) };
    }
}