#include "application.h"

// TODO: move somewhere more meaningful
// GLM init/include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "construct_array.h"
#include "mdsp_common/wrapper.h"

namespace terracota
{
    using namespace cisim;

    namespace
    {
        vk::Extent2D window_extent(const cen::window& window)
        {
            auto size = cen::cast<uint32_t>(window.size());

            return vk::Extent2D{ size.width, size.height };
        }

        // move to swapchain or somewhere more appropriate
        vk::ImageSubresourceRange image_subresource_range(vk::ImageAspectFlags aspect_mask)
        {
            // similar to dx11 subresource box
            return vk::ImageSubresourceRange{
                .aspectMask = aspect_mask,
                .baseMipLevel = 0,
                .levelCount = vk::RemainingMipLevels,
                .baseArrayLayer = 0,
                .layerCount = vk::RemainingArrayLayers
            };
        }

        // move to swapchain or somewhere more appropriate
        void transition_image(vk::raii::CommandBuffer& cmd, vk::Image& image, vk::ImageLayout old_layout, vk::ImageLayout new_layout)
        {
            vk::ImageMemoryBarrier2 barrier{
                .srcStageMask = vk::PipelineStageFlagBits2::eAllCommands,
                .srcAccessMask = vk::AccessFlagBits2::eMemoryWrite,
                .dstStageMask = vk::PipelineStageFlagBits2::eAllCommands,
                .dstAccessMask = vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
                .oldLayout = old_layout,
                .newLayout = new_layout,
                .image = image,
                .subresourceRange = image_subresource_range(
                    new_layout == vk::ImageLayout::eDepthAttachmentOptimal
                        ? vk::ImageAspectFlagBits::eDepth
                        : vk::ImageAspectFlagBits::eColor)
            };

            cmd.pipelineBarrier2(vk::DependencyInfo{}
                .setImageMemoryBarriers(barrier));
        }
    }

    application::v::v(const cen::window& window, vk::context& context)
        : context{ context }
        , queues{ context }
        , swap_chain{ context, window_extent(window) }
        , frames{ construct_array<vk::frame_data, vk::FRAME_OVERLAP>(context.device, context.qi) }
        //, pipeline{ context.device, swap_chain,
        //    vk::pipeline::params{
        //        window_extent(window),
        //        vk::PrimitiveTopology::eTriangleList,
        //        vk::PolygonMode::eFill,
        //        {
        //            { vk::ShaderStageFlagBits::eVertex, shaders::read("shaders/shader.vert.spv") },
        //            { vk::ShaderStageFlagBits::eFragment, shaders::read("shaders/shader.frag.spv") },
        //        }
        //    }
        //}
    {
    }

    vk::frame_data& application::v::current_frame()
    {
        return frames[_current_frame_idx];
    }

    void application::v::swap()
    {
        _current_frame_idx = (_current_frame_idx + 1) % vk::FRAME_OVERLAP;
        frames_drawn++;
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

            // TODO: fix this bullshit productions
            auto& device = _vulkan.context.device;
            auto& current_frame = _vulkan.current_frame();
            auto& swapchain = _vulkan.swap_chain();
            auto images = swapchain.getImages();
            auto& frames_drawn = _vulkan.frames_drawn;

            // ----- RECORD COMMAND BUFFER -----

            // Wait until the gpu has finished rendering the last frame. Timeout of 1 second
            Time timeout{ 1_s };
            while (vk::Result::eTimeout == device.waitForFences(*current_frame.command_fence, true, timeout.nanoseconds<uint64_t>()));

            device.resetFences(*current_frame.command_fence);
            auto [result, framebuffer_index] = swapchain.acquireNextImage(timeout.nanoseconds<uint64_t>(), current_frame.swapchain_semaphore);
            auto& current_image = images[framebuffer_index];

            // Using buffers.front as we are using only 1 command buffer
            auto& command_buffer = current_frame.buffers.front();
            command_buffer.reset();
            command_buffer.begin(vk::CommandBufferBeginInfo{
                .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            });

            transition_image(command_buffer, current_image,
                vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral);

            float flash = std::abs(std::sin(frames_drawn / 120.f));
            vk::ClearColorValue clear_value{ std::array{ 1.f - flash, 0.f, flash, 1.0f } };

            auto clear_range = image_subresource_range(vk::ImageAspectFlagBits::eColor);
            command_buffer.clearColorImage(current_image, vk::ImageLayout::eGeneral, clear_value, clear_range);

            transition_image(command_buffer, current_image, vk::ImageLayout::eGeneral, vk::ImageLayout::ePresentSrcKHR);

            command_buffer.end();

            // ----- SUBMIT COMMAND BUFFER -----

            vk::CommandBufferSubmitInfo command_buffer_info{
                .commandBuffer = command_buffer
            };

            vk::SemaphoreSubmitInfo wait_info{
                .semaphore = current_frame.swapchain_semaphore,
                .value = 1,
                .stageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .deviceIndex = 0
            };

            vk::SemaphoreSubmitInfo signal_info{
                .semaphore = current_frame.present_semaphore,
                .value = 1,
                .stageMask = vk::PipelineStageFlagBits2::eAllGraphics,
                .deviceIndex = 0
            };

            auto& graphics_queue = _vulkan.queues.queue(vk::queue_family::graphics);
            auto& present_queue = _vulkan.queues.queue(vk::queue_family::presentation);

            graphics_queue.submit2(
                vk::SubmitInfo2{}
                    .setCommandBufferInfos(command_buffer_info)
                    .setWaitSemaphoreInfos(wait_info)
                    .setSignalSemaphoreInfos(signal_info), current_frame.command_fence);

            present_queue.presentKHR(vk::PresentInfoKHR{}
                .setImageIndices(framebuffer_index)
                .setSwapchains(*swapchain)
                .setWaitSemaphores(*current_frame.present_semaphore)
            );

            _vulkan.swap();

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
        _vulkan.context.device.waitIdle();
        _centurion.window.hide();
    }

    void application::run()
    {
        loop();
    }
}
