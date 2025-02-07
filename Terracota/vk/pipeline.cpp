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

        static constexpr auto color_blend_attachment = vk::PipelineColorBlendAttachmentState{
            .blendEnable = true,
            .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor = vk::BlendFactor::eZero,
            .alphaBlendOp = vk::BlendOp::eAdd,
            .colorWriteMask =
                vk::ColorComponentFlagBits::eR |
                vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA,
        };
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
            static auto info = vk::PipelineInputAssemblyStateCreateInfo{
                .primitiveRestartEnable = false };

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

        vk::PipelineMultisampleStateCreateInfo& multisample()
        {
            // TODO: designated initializers
            static auto info = vk::PipelineMultisampleStateCreateInfo{
                .rasterizationSamples = vk::SampleCountFlagBits::e1,
                .sampleShadingEnable = false,
                .minSampleShading = 1.f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = false,
                .alphaToOneEnable = false,
            };

            return info;
        }

        vk::PipelineColorBlendStateCreateInfo& color_blend()
        {
            // TODO: designated initializers
            static auto info =  vk::PipelineColorBlendStateCreateInfo{}
                .setLogicOpEnable(false)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(color_blend_attachment)
                .setBlendConstants({ 0.f, 0.f, 0.f, 0.f });

            return info;
        }
    }

    namespace description
    {
        vk::AttachmentDescription& color_attachment(vk::Format color_format)
        {
            // TODO: designated initializers
            static auto description = vk::AttachmentDescription{}
                .setFormat(color_format)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

            return description;
        }

        // TODO: multiple attachments (color, depth, stencil, post-process)
        std::array<vk::AttachmentDescription, 1>& attachment_array(vk::Format color_format)
        {
            static auto array = std::array{
                color_attachment(color_format)
            };

            return array;
        }

        vk::SubpassDescription& subpass(uint32_t attachment_index)
        {
            // TODO: designated initializers
            static auto reference = vk::AttachmentReference{}
                .setAttachment(attachment_index)
                .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

            static auto reference_array = std::array{ reference };

            // TODO: designated initializers
            static auto description = vk::SubpassDescription{}
                .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                .setColorAttachments(reference_array);

            return description;
        }

        // TODO: same here, multiple subpasses
        std::array <vk::SubpassDescription, 1>& subpass_array(uint32_t attachment_index)
        {
            static auto array = std::array{
                subpass(attachment_index)
            };

            return array;
        }
    }

    pipeline::pipeline(vk::raii::Device& device, vk::swap_chain& swap_chain, const params& params)
        : _ssi{ device, params.shaders }
        , _vpi{ params.extent }
        , _layout{ device,
            vk::PipelineLayoutCreateInfo{}
                .setSetLayouts(nullptr)
                .setPushConstantRanges(nullptr) }
        , _render_pass{ device,
            vk::RenderPassCreateInfo{}
                .setAttachments(description::attachment_array(swap_chain.info()().imageFormat))
                .setSubpasses(description::subpass_array(0)) }
        , _pipeline{ device, nullptr,
            vk::GraphicsPipelineCreateInfo{}
                .setStages(_ssi())
                .setPVertexInputState(&info::vertex_input())
                .setPInputAssemblyState(&info::input_assembly(params.topology))
                .setPViewportState(&_vpi())
                .setPRasterizationState(&info::rasterization(params.polygon_mode))
                .setPMultisampleState(&info::multisample())
                .setPColorBlendState(&info::color_blend())
                .setPDynamicState(&info::dynamic())
                .setLayout(_layout)
                .setRenderPass(_render_pass)
                .setSubpass(0) }
    {
        _framebuffers.reserve(swap_chain.framebuffer_views().size());

        std::array<vk::ImageView, 1> attachments;

        vk::FramebufferCreateInfo info{
            .renderPass = *_render_pass,
            .width = params.extent.width,
            .height = params.extent.height,
            .layers = 1
        };

        for (auto const& view : swap_chain.framebuffer_views())
        {
            attachments[0] = view;
            _framebuffers.push_back(vk::raii::Framebuffer{ device, info.setAttachments(attachments) });
        }
    }
}