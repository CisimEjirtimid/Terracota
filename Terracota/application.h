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

#include "vk/instance_info.h"
#include "vk/device_info.h"
#include "vk/swap_chain_info.h"

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

            vk::device_info di;
            vk::raii::Device device;

            vk::raii::Queue graphics_queue;
            vk::raii::Queue presentation_queue;
            vk::raii::Queue compute_queue;

            vk::swap_chain_info sci;
            //vk::raii::SwapchainKHR swap_chain;

            v(cen::window& window, const vk::instance_info& params);
        } _vulkan;

        void loop();

    public:
        application();
        ~application();

        void run();
    };
}

