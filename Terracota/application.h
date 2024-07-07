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

                vk::InstanceCreateInfo instance_info;

                vk::PhysicalDeviceFeatures physical_device_features;
                vk::DeviceQueueCreateInfo queue_info;
                vk::DeviceCreateInfo device_info;

                construct_params(const vk::ApplicationInfo& application_info);
            };

            struct device_create_info
            {
                // idx 0: graphics, idx 1: compute
                static constexpr size_t qi_graphics_idx = 0;
                static constexpr size_t qi_compute_idx = 1;

                std::array<vk::DeviceQueueCreateInfo, 2> queue_infos;
                std::array<float, 2> queue_priorities{ 1.f, 1.f };

                vk::PhysicalDeviceFeatures physical_device_features;

                vk::DeviceCreateInfo device_info;

                device_create_info(vk::raii::PhysicalDevice& physical_device);
            };

            vk::raii::Context context;
            vk::raii::Instance instance;
            vk::raii::SurfaceKHR surface;

            vk::raii::PhysicalDevice physical_device;

            device_create_info dci; // used to create the device
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

