/*
 * Vulkan Windowed Program
 *
 * Copyright (C) 2016, 2018 Valve Corporation
 * Copyright (C) 2016, 2018 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
Vulkan C++ Windowed Project Template
Create and destroy a Vulkan surface on an SDL window.
*/

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX
#endif

// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <iostream>
#include <vector>

#define CENTURION_NO_SDL_IMAGE
#define CENTURION_NO_SDL_MIXER
#define CENTURION_NO_SDL_TTF
#define CENTURION_NO_OPENGL
#include <centurion/centurion.hpp>

int main()
{
    try
    {
        // Create an SDL window that supports Vulkan rendering.
        cen::sdl sdl{ cen::sdl_cfg{ SDL_INIT_VIDEO } };

        cen::window window{ "Vulkan Window", cen::iarea{ 1280, 720 }, cen::window::window_flags::vulkan };

        // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
        auto extensions = cen::vk::required_extensions();

        if (!extensions)
            throw std::runtime_error{ "No Vulkan Extensions!" };

        // Use validation layers if this is a debug build
        std::vector<const char*> layers;
#if defined(_DEBUG)
        layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        vk::raii::Context context;

        // vk::ApplicationInfo allows the programmer to specifiy some basic information about the
        // program, which can be useful for layers and tools to provide more debug information.
        vk::ApplicationInfo app_info = vk::ApplicationInfo()
            .setPApplicationName("Vulkan C++ Windowed Program Template")
            .setApplicationVersion(1)
            .setPEngineName("LunarG SDK")
            .setEngineVersion(1)
            .setApiVersion(VK_API_VERSION_1_0);

        // vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
        // are needed.
        vk::InstanceCreateInfo instance_info = vk::InstanceCreateInfo()
            .setFlags(vk::InstanceCreateFlags())
            .setPApplicationInfo(&app_info)
            .setEnabledExtensionCount(static_cast<uint32_t>(extensions->size()))
            .setPpEnabledExtensionNames(extensions->data())
            .setEnabledLayerCount(static_cast<uint32_t>(layers.size()))
            .setPpEnabledLayerNames(layers.data());

        // Create the Vulkan instance.
        vk::raii::Instance instance{ context, instance_info };

        // Create a Vulkan surface for rendering
        VkSurfaceKHR raw_surface;
        cen::vk::make_surface(window, *instance, &raw_surface);
        vk::raii::SurfaceKHR surface{ instance, raw_surface };

        // This is where most initializtion for a program should be performed

        window.show();

        bool running = true;
        while (running)
        {
            cen::event_handler event;

            // Poll for user input
            while (event.poll())
            {
                // Handle events
                if (event.is(cen::event_type::quit))
                {
                    running = false;
                }
            }

            // This is where render, data update would go
        }

        window.hide();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
