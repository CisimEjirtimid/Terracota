#include "application.h"

// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <iostream>

#include "strcmp_functor.h"

namespace terracota
{
    // TODO: customization options for `application_info` and `instance_info` functions
    namespace
    {
        vk::ApplicationInfo application_info(const std::string& title)
        {
            // vk::ApplicationInfo allows the programmer to specifiy some basic information about the
            // program, which can be useful for layers and tools to provide more debug information.
            return vk::ApplicationInfo()
                .setPApplicationName(title.c_str())
                .setApplicationVersion(1)
                .setPEngineName("Terracota Rendering Engine")
                .setEngineVersion(1)
                .setApiVersion(VK_API_VERSION_1_0);
        }   

        // Create a Vulkan surface for rendering
        VkSurfaceKHR raw_surface(cen::window& window, vk::raii::Instance& instance)
        {
            VkSurfaceKHR raw;
            cen::vk::make_surface(window, *instance, &raw);
            return std::move(raw);
        }
    }

    Application::C::C(const std::string& title, const cen::iarea& size)
        : context{ cen::sdl_cfg{ SDL_INIT_VIDEO } }
        , window{
            title.c_str(),
            size,
            cen::window::window_flags::vulkan
        }
    {
    }

    Application::V::ConstructParams::ConstructParams(const vk::ApplicationInfo& application_info)
    {
        // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
        auto maybe_extensions = cen::vk::required_extensions();

        if (!maybe_extensions)
            throw std::runtime_error{ "No Vulkan extensions!" };

        vk::name_vector required_extensions{ *maybe_extensions };
        std::sort(required_extensions.begin(), required_extensions.end(), cisim::strcmp_functor{});

        auto available_extensions = vk::extensions();
        std::sort(available_extensions.begin(), available_extensions.end(), cisim::strcmp_functor{});

        std::set_intersection(
            required_extensions.begin(), required_extensions.end(),
            available_extensions.begin(), available_extensions.end(),
            std::back_inserter(extensions), cisim::strcmp_functor{});

        if (required_extensions.size() != extensions.size())
            throw std::runtime_error{ "Required extensions not supported!" };

        // Use validation layers if this is a debug build
#if defined(_DEBUG)
        layers.push_back(std::string_view{ "VK_LAYER_KHRONOS_validation" });
#endif

        // vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
        // are needed.
        info = vk::InstanceCreateInfo()
            .setFlags(vk::InstanceCreateFlags())
            .setPApplicationInfo(&application_info)
            .setPEnabledExtensionNames(extensions.native())
            .setPEnabledLayerNames(layers.native());
    }

    Application::V::V(cen::window& window, const Application::V::ConstructParams& params)
        : instance{ context, params.info }
        , surface{ instance, raw_surface(window, instance) }
    {}

    void Application::loop()
    {
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
    }

    Application::Application()
        : _centurion{ "Terracota", cen::iarea{ 1280, 720 } }
        , _vulkan{ _centurion.window, Application::V::ConstructParams{ application_info("Terracota") } }
    {
        // This is where rest of initializtion for a program should be performed

        _centurion.window.show();
    }

    Application::~Application()
    {
        _centurion.window.hide();
    }

    void Application::run()
    {
        loop();
    }
}
