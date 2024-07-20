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

#include "vk/construct_params.h"
#include "vk/device_create_info.h"

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
            vk::raii::Context context;
            vk::raii::Instance instance;
            vk::raii::SurfaceKHR surface;

            vk::raii::PhysicalDevice physical_device;

            vk::device_create_info dci; // used to create the device
            vk::raii::Device device;

            vk::raii::Queue graphics_queue;
            vk::raii::Queue compute_queue;

            v(cen::window& window, const vk::construct_params& params);
        } _vulkan;

        void loop();

    public:
        application();
        ~application();

        void run();
    };
}

