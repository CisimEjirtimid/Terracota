#include "application.h"

// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <iostream>
#include <execution>
#include <type_traits>

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

        vk::raii::PhysicalDevice pick_physical_device(vk::raii::Instance& instance)
        {
            auto valid = [](vk::raii::PhysicalDevice& physical_device)
            {
                auto properties = physical_device.getProperties();
                auto features = physical_device.getFeatures();

                auto qfps = physical_device.getQueueFamilyProperties();

                auto queue_flag_predicate = [](vk::QueueFlagBits flag_bit)
                {
                    return [flag_bit](vk::QueueFamilyProperties& qfp) -> bool
                    {
                        return (qfp.queueFlags & flag_bit) == flag_bit;
                    };
                };

                using flag_predicate = std::invoke_result_t<decltype(queue_flag_predicate), vk::QueueFlagBits>;

                auto exists_queue = [](std::vector<vk::QueueFamilyProperties>& qfps, flag_predicate&& predicate)
                {
                    return std::find_if(std::execution::par, qfps.begin(), qfps.end(), predicate) != qfps.end();
                };

                auto graphics_queue = exists_queue(qfps, queue_flag_predicate(vk::QueueFlagBits::eGraphics));
                auto compute_queue = exists_queue(qfps, queue_flag_predicate(vk::QueueFlagBits::eCompute));

                // TODO: make these requirements configurable somehow
                return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu
                    && features.shaderFloat64 != 0 // bools are uint32_t
                    && graphics_queue && compute_queue; // these are proper bool
            };

            for (auto& physical_device : vk::raii::PhysicalDevices{ instance })
                if (valid(physical_device))
                    return physical_device;

            throw std::runtime_error{ "No valid physical device found!" };
        }
    }

    application::c::c(const std::string& title, const cen::iarea& size)
        : context{ cen::sdl_cfg{ SDL_INIT_VIDEO } }
        , window{
            title.c_str(),
            size,
            cen::window::window_flags::vulkan
        }
    {
    }

    application::v::construct_params::construct_params(const vk::ApplicationInfo& application_info)
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

    application::v::v(cen::window& window, const application::v::construct_params& params)
        : instance{ context, params.info }
        , surface{ instance, raw_surface(window, instance) }
        , device{ pick_physical_device(instance), vk::DeviceCreateInfo{} } // TODO: provide vk::DeviceCreateInfo
    {
    }

    void application::loop()
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

    application::application()
        : _centurion{ "Terracota", cen::iarea{ 1280, 720 } }
        , _vulkan{ _centurion.window, application::v::construct_params{ application_info("Terracota") } }
    {
        // This is where rest of initializtion for a program should be performed

        _centurion.window.show();
    }

    application::~application()
    {
        _centurion.window.hide();
    }

    void application::run()
    {
        loop();
    }
}
