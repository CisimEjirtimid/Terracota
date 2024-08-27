#include "application.h"

// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <iostream>
#include <execution>
#include <type_traits>
#include <array>

#include "shaders/utils.h"

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
    {
        // TODO: move to pipeline.h/.cpp
        auto vertex_shader = shaders::read("shaders/shader.vert.spv");
        auto fragment_shader = shaders::read("shaders/shader.frag.spv");
        vk::raii::ShaderModule vertex_shader_module{ context.device, vk::ShaderModuleCreateInfo{}.setCode(vertex_shader) };
        vk::raii::ShaderModule fragment_shader_module{ context.device, vk::ShaderModuleCreateInfo{}.setCode(fragment_shader) };

        //std::array<vk::PipelineShaderStageCreateInfo, 2> shader_stages_info{
        //    vk::PipelineShaderStageCreateInfo{}
        //        .setStage(vk::ShaderStageFlagBits::eVertex)
        //        .setModule(vertex_shader_module)
        //        .setPName("main"),
        //        vk::PipelineShaderStageCreateInfo{}
        //        .setStage(vk::ShaderStageFlagBits::eFragment)
        //        .setModule(fragment_shader_module)
        //        .setPName("main")
        //};

        //std::array<vk::DynamicState, 2> dynamic_states{
        //    vk::DynamicState::eViewport,
        //    vk::DynamicState::eScissor
        //};
        //auto dynamic_state_info = vk::PipelineDynamicStateCreateInfo{}
        //    .setDynamicStates(dynamic_states);

        //vk::raii::Pipeline pipeline{ context.device, nullptr,
        //    vk::GraphicsPipelineCreateInfo{}
        //        .setStages(shader_stages_info)
        //        .setPDynamicState(&dynamic_state_info)};
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
