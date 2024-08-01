#pragma once

#include "cen/utils.h"

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

            cen::u32area framebuffer_size;
            vk::swap_chain_info sci;
            vk::raii::SwapchainKHR swap_chain;
            std::vector<vk::raii::ImageView> framebuffer_views;

            v(cen::window& window, const vk::instance_info& params);
        } _vulkan;

        void loop();

    public:
        application();
        ~application();

        void run();
    };
}

