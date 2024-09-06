#include "pipeline.h"

namespace terracota::vk
{
    pipeline::pipeline(vk::raii::Device& device, const vk::Extent2D& extent)
    {
        // TODO: parametrize shaders
        auto vertex_shader = shaders::read("shaders/shader.vert.spv");
        auto fragment_shader = shaders::read("shaders/shader.frag.spv");
        vk::raii::ShaderModule vertex_shader_module{ device, vk::ShaderModuleCreateInfo{}.setCode(vertex_shader) };
        vk::raii::ShaderModule fragment_shader_module{ device, vk::ShaderModuleCreateInfo{}.setCode(fragment_shader) };

        std::array<vk::PipelineShaderStageCreateInfo, 2> shader_stages_info{
            vk::PipelineShaderStageCreateInfo{}
                .setStage(vk::ShaderStageFlagBits::eVertex)
                .setModule(vertex_shader_module)
                .setPName("main"),
                vk::PipelineShaderStageCreateInfo{}
                .setStage(vk::ShaderStageFlagBits::eFragment)
                .setModule(fragment_shader_module)
                .setPName("main")
        };

        std::array<vk::DynamicState, 2> dynamic_state_array{
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };
        auto dynamic_state_info = vk::PipelineDynamicStateCreateInfo{}
            .setDynamicStates(dynamic_state_array);

        auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo{}
            .setVertexBindingDescriptions({}) 
            .setVertexAttributeDescriptions({});

        auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo{}
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(false);

        auto viewport = vk::Viewport{}
            .setX(0)
            .setY(0)
            .setWidth(extent.width)
            .setHeight(extent.height)
            .setMinDepth(0.f)
            .setMaxDepth(1.f);

        auto scissor = vk::Rect2D{}
            .setOffset({ 0, 0 })
            .setExtent(extent);

        auto viewport_info = vk::PipelineViewportStateCreateInfo{}
            .setScissors(scissor)
            .setViewports(viewport);

        auto rasterizer_info = vk::PipelineRasterizationStateCreateInfo{}
            .setDepthClampEnable(false)
            .setRasterizerDiscardEnable(false)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.f)
            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eClockwise)
            .setDepthBiasEnable(false)
            .setDepthBiasConstantFactor(0.f)//
            .setDepthBiasClamp(0.f)         // Optional
            .setDepthBiasSlopeFactor(0.f);  //

        auto color_blend_attachment_info = vk::PipelineColorBlendAttachmentState{}
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

        auto color_blend_info = vk::PipelineColorBlendStateCreateInfo{}
            .setLogicOpEnable(false)
            .setLogicOp(vk::LogicOp::eCopy)
            .setAttachments(color_blend_attachment_info)
            .setBlendConstants({ 0.f, 0.f, 0.f, 0.f });

        // TODO: init _pipeline with this
        vk::raii::Pipeline pipeline{ device, nullptr,
            vk::GraphicsPipelineCreateInfo{}
                .setStages(shader_stages_info)
                .setPDynamicState(&dynamic_state_info)
                .setPVertexInputState(&vertex_input_info)
                .setPInputAssemblyState(&input_assembly_info)
                .setPViewportState(&viewport_info)
                .setPRasterizationState(&rasterizer_info)
                .setPColorBlendState(&color_blend_info)
        };

        // TODO: init _layout with this
        vk::raii::PipelineLayout layout{ device,
            vk::PipelineLayoutCreateInfo{}
                .setSetLayouts({})
                .setPushConstantRanges({})
        };
    }
}