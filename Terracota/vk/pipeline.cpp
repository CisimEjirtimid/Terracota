#include "pipeline.h"
#include <map>

namespace terracota::vk
{
    namespace
    {
        static constexpr auto dynamic_state_array = std::array
        {
            DynamicState::eViewport,
            DynamicState::eScissor
        };

        static constexpr auto color_blend_attachment = PipelineColorBlendAttachmentState{
            .blendEnable = true,
            .srcColorBlendFactor = BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp = BlendOp::eAdd,
            .srcAlphaBlendFactor = BlendFactor::eOne,
            .dstAlphaBlendFactor = BlendFactor::eZero,
            .alphaBlendOp = BlendOp::eAdd,
            .colorWriteMask =
                ColorComponentFlagBits::eR |
                ColorComponentFlagBits::eG |
                ColorComponentFlagBits::eB |
                ColorComponentFlagBits::eA,
        };
    }

    namespace info
    {
        PipelineDynamicStateCreateInfo& dynamic()
        {
            static auto info = PipelineDynamicStateCreateInfo{}
                .setDynamicStates(dynamic_state_array);

            return info;
        }

        PipelineVertexInputStateCreateInfo& vertex_input()
        {
            static auto info = PipelineVertexInputStateCreateInfo{}
                .setVertexBindingDescriptions({})
                .setVertexAttributeDescriptions({});

            return info;
        }

        PipelineInputAssemblyStateCreateInfo& input_assembly(const PrimitiveTopology& topology)
        {
            static auto info = PipelineInputAssemblyStateCreateInfo{
                .primitiveRestartEnable = false };

            return info.setTopology(topology);
        }

        PipelineRasterizationStateCreateInfo& rasterization(const PolygonMode& mode)
        {
            static auto info = PipelineRasterizationStateCreateInfo{}
                .setDepthClampEnable(false)
                .setRasterizerDiscardEnable(false)
                .setPolygonMode(mode)
                .setLineWidth(1.f)
                .setCullMode(CullModeFlagBits::eBack)   // Parametrize
                .setFrontFace(FrontFace::eClockwise)    //
                .setDepthBiasEnable(false)
                .setDepthBiasConstantFactor(0.f)//
                .setDepthBiasClamp(0.f)         // Optional
                .setDepthBiasSlopeFactor(0.f);  //

            return info.setPolygonMode(mode);
        }

        PipelineMultisampleStateCreateInfo& multisample()
        {
            // TODO: designated initializers
            static auto info = PipelineMultisampleStateCreateInfo{
                .rasterizationSamples = SampleCountFlagBits::e1,
                .sampleShadingEnable = false,
                .minSampleShading = 1.f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = false,
                .alphaToOneEnable = false,
            };

            return info;
        }

        PipelineColorBlendStateCreateInfo& color_blend()
        {
            // TODO: designated initializers
            static auto info =  PipelineColorBlendStateCreateInfo{}
                .setLogicOpEnable(false)
                .setLogicOp(LogicOp::eCopy)
                .setAttachments(color_blend_attachment)
                .setBlendConstants({ 0.f, 0.f, 0.f, 0.f });

            return info;
        }
    }

    namespace description
    {
        AttachmentDescription& color_attachment(Format color_format)
        {
            // TODO: designated initializers
            static auto description = AttachmentDescription{}
                .setFormat(color_format)
                .setSamples(SampleCountFlagBits::e1)
                .setLoadOp(AttachmentLoadOp::eClear)
                .setStoreOp(AttachmentStoreOp::eStore)
                .setStencilLoadOp(AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(AttachmentStoreOp::eDontCare)
                .setInitialLayout(ImageLayout::eUndefined)
                .setFinalLayout(ImageLayout::ePresentSrcKHR);

            return description;
        }

        // TODO: multiple attachments (color, depth, stencil, post-process)
        std::array<AttachmentDescription, 1>& attachment_array(Format color_format)
        {
            static auto array = std::array{
                color_attachment(color_format)
            };

            return array;
        }

        SubpassDescription& subpass(uint32_t attachment_index)
        {
            // TODO: designated initializers
            static auto reference = AttachmentReference{}
                .setAttachment(attachment_index)
                .setLayout(ImageLayout::eColorAttachmentOptimal);

            static auto reference_array = std::array{ reference };

            // TODO: designated initializers
            static auto description = SubpassDescription{}
                .setPipelineBindPoint(PipelineBindPoint::eGraphics)
                .setColorAttachments(reference_array);

            return description;
        }

        // TODO: same here, multiple subpasses
        std::array <SubpassDescription, 1>& subpass_array(uint32_t attachment_index)
        {
            static auto array = std::array{
                subpass(attachment_index)
            };

            return array;
        }
    }

    pipeline::pipeline(raii::Device& device, swap_chain& swap_chain, const params& params)
        : _ssi{ device, params.shaders }
        , _vpi{ params.extent }
        , _layout{ device,
            PipelineLayoutCreateInfo{}
                .setSetLayouts(nullptr)
                .setPushConstantRanges(nullptr) }
        , _render_pass{ device,
            RenderPassCreateInfo{}
                .setAttachments(description::attachment_array(swap_chain.info()().imageFormat))
                .setSubpasses(description::subpass_array(0)) }
        , _pipeline{ device, nullptr,
            GraphicsPipelineCreateInfo{}
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

        std::array<ImageView, 1> attachments;

        FramebufferCreateInfo info{
            .renderPass = *_render_pass,
            .width = params.extent.width,
            .height = params.extent.height,
            .layers = 1
        };

        for (auto const& view : swap_chain.framebuffer_views())
        {
            attachments[0] = view;
            _framebuffers.push_back(raii::Framebuffer{ device, info.setAttachments(attachments) });
        }
    }
}