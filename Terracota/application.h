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

#include "vk/utils.h"

namespace terracota
{
    class Application
    {
        struct C
        {
            cen::sdl context;
            cen::window window;

            C(const std::string& title, const cen::iarea& size);
        } _centurion;

        struct V
        {
            struct ConstructParams
            {
                vk::name_vector extensions;
                vk::name_vector layers;

                vk::InstanceCreateInfo info;

                ConstructParams(const vk::ApplicationInfo& application_info);
            };

            vk::raii::Context context;
            vk::raii::Instance instance;
            vk::raii::SurfaceKHR surface;

            V(cen::window& window, const ConstructParams& params);
        } _vulkan;

        void loop();

    public:
        Application();
        ~Application();

        void run();
    };
}

