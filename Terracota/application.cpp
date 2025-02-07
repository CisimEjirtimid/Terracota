#include "application.h"

// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <iostream>
#include <execution>
#include <type_traits>
#include <array>

namespace terracota
{
    namespace
    {
        vk::Extent2D window_extent(const cen::window& window)
        {
            auto size = cen::cast<uint32_t>(window.size());

            return vk::Extent2D{ size.width, size.height };
        }
    }

    application::v::v(const cen::window& window, vk::context& context)
        : context{ context }
        , queues{ context }
        , swap_chain{ context, window_extent(window) }
        , pipeline{ context.device, swap_chain,
            vk::pipeline::params{
                window_extent(window),
                vk::PrimitiveTopology::eTriangleList,
                vk::PolygonMode::eFill,
                {
                    { vk::ShaderStageFlagBits::eVertex, shaders::read("shaders/shader.vert.spv") },
                    { vk::ShaderStageFlagBits::eFragment, shaders::read("shaders/shader.frag.spv") },
                }
            }
        }
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

    application::application(cen::context& centurion, vk::context& vk)
        : _centurion(centurion)
        , _vulkan{ _centurion.window, vk }
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
