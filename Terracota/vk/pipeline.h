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

        raii::RenderPass _render_pass;
        raii::Pipeline _pipeline;
        raii::PipelineLayout _layout;

        std::vector<raii::Framebuffer> _framebuffers;

    public:
        struct params
        {
            Extent2D extent;
            PrimitiveTopology topology;
            PolygonMode polygon_mode;

            std::map<ShaderStageFlagBits, shaders::descriptor> shaders;
        };

        pipeline(raii::Device& device, swap_chain& swap_chain, const params& params);
    };
}