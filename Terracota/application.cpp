#include "application.h"

// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <iostream>
#include <execution>
#include <type_traits>

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
                        return bool(qfp.queueFlags & flag_bit);
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
                    && features.shaderFloat64 // bools are uint32_t
                    && features.tessellationShader
                    && features.geometryShader
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

    application::v::v(cen::window& window, const vk::instance_info& ii)
        : instance{ context, ii.info }
        , surface{ instance, raw_surface(window, instance) }
        , physical_device{ pick_physical_device(instance) }
        , di{ physical_device, surface }
        , device{ physical_device, di.info }
        , graphics_queue{ device, di.queue_family_indices[vk::queue_info_index::graphics], 0 }
        , presentation_queue{ device, di.queue_family_indices[vk::queue_info_index::presentation], 0 }
        , compute_queue{ device, di.queue_family_indices[vk::queue_info_index::compute], 0 }
        , sci{ physical_device, surface }
        //, swap_chain{ device, sci.info }
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
        , _vulkan{
            _centurion.window,
            vk::instance_info{
                application_info("Terracota"),
                cen::vk::required_extensions()
            }
        }
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
