#pragma once
#include <map>
#include "swap_chain.h"
#include "viewport_info.h"
#include "shader_stages_info.h"

namespace terracota::vk
{
    class pipeline
    {
        shader_stages_info _ssi;
        viewport_info _vpi;

        vk::raii::RenderPass _render_pass;
        vk::raii::Pipeline _pipeline;
        vk::raii::PipelineLayout _layout;

        std::vector<vk::raii::Framebuffer> _framebuffers;

    public:
        struct params
        {
            vk::Extent2D extent;
            vk::PrimitiveTopology topology;
            vk::PolygonMode polygon_mode;

            std::map<vk::ShaderStageFlagBits, shaders::descriptor> shaders;
        };

        pipeline(vk::raii::Device& device, vk::swap_chain& swap_chain, const params& params);
    };
}