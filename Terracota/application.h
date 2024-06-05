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
    class application
    {
        struct c
        {
            cen::sdl context;
            cen::window window;

            c(const std::string& title, const cen::iarea& size);
        } _centurion;

        struct v
        {
            struct construct_params
            {
                vk::name_vector extensions;
                vk::name_vector layers;

                vk::InstanceCreateInfo info;

                construct_params(const vk::ApplicationInfo& application_info);
            };

            vk::raii::Context context;
            vk::raii::Instance instance;
            vk::raii::SurfaceKHR surface;

            vk::raii::Device device;

            v(cen::window& window, const construct_params& params);
        } _vulkan;

        void loop();

    public:
        application();
        ~application();

        void run();
    };
}

