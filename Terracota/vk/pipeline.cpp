#include "pipeline.h"
#include <map>

namespace terracota::vk
{
    namespace
    {
        static constexpr auto dynamic_state_array = std::array
        {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        static constexpr auto color_blend_attachment = vk::PipelineColorBlendAttachmentState{}
            .setColorWriteMask(
                vk::ColorComponentFlagBits::eR |
                vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA)
            .setBlendEnable(true)
            .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
            .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
            .setColorBlendOp(vk::BlendOp::eAdd)
            .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
            .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
            .setAlphaBlendOp(vk::BlendOp::eAdd);
    }

    namespace info
    {
        vk::PipelineDynamicStateCreateInfo& dynamic()
        {
            static auto info = vk::PipelineDynamicStateCreateInfo{}
                .setDynamicStates(dynamic_state_array);

            return info;
        }

        vk::PipelineVertexInputStateCreateInfo& vertex_input()
        {
            static auto info = vk::PipelineVertexInputStateCreateInfo{}
                .setVertexBindingDescriptions({})
                .setVertexAttributeDescriptions({});

            return info;
        }

        vk::PipelineInputAssemblyStateCreateInfo& input_assembly(const vk::PrimitiveTopology& topology)
        {
            static auto info = vk::PipelineInputAssemblyStateCreateInfo{}
                .setPrimitiveRestartEnable(false);

            return info.setTopology(topology);
        }

        vk::PipelineRasterizationStateCreateInfo& rasterization(const vk::PolygonMode& mode)
        {
            static auto info = vk::PipelineRasterizationStateCreateInfo{}
                .setDepthClampEnable(false)
                .setRasterizerDiscardEnable(false)
                .setPolygonMode(mode)
                .setLineWidth(1.f)
                .setCullMode(vk::CullModeFlagBits::eBack)   // Parametrize
                .setFrontFace(vk::FrontFace::eClockwise)    //
                .setDepthBiasEnable(false)
                .setDepthBiasConstantFactor(0.f)//
                .setDepthBiasClamp(0.f)         // Optional
                .setDepthBiasSlopeFactor(0.f);  //

            return info.setPolygonMode(mode);
        }

        vk::PipelineColorBlendStateCreateInfo& color_blend()
        {
            static auto info =  vk::PipelineColorBlendStateCreateInfo{}
                .setLogicOpEnable(false)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(color_blend_attachment)
                .setBlendConstants({ 0.f, 0.f, 0.f, 0.f });

            return info;
        }
    }

    pipeline::pipeline(vk::raii::Device& device, const params& params)
        : _ssi{ device, params.shaders }
        , _vpi{ params.extent }
        , _layout{ device,
            vk::PipelineLayoutCreateInfo{}
                .setSetLayouts({})
                .setPushConstantRanges({}) }
        , _pipeline{ device, nullptr,
            vk::GraphicsPipelineCreateInfo{}
                .setStages(_ssi())
                .setPDynamicState(&info::dynamic())
                .setPVertexInputState(&info::vertex_input())
                .setPInputAssemblyState(&info::input_assembly(params.topology))
                .setPViewportState(&_vpi())
                .setPRasterizationState(&info::rasterization(params.polygon_mode))
                .setPColorBlendState(&info::color_blend())
                .setLayout(_layout) }
    {
    }
}