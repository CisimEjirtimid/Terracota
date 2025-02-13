#pragma once
#include "context.h"
#include "swap_chain_info.h"

namespace terracota::vk
{
    // TODO: rename to swapchain, without _
    class swap_chain
    {
        swap_chain_info _info;
        raii::SwapchainKHR _swap_chain;
        std::vector<raii::ImageView> _framebuffer_views;

    public:
        swap_chain(context& context, const Extent2D& framebuffer_size);

        const swap_chain_info& info() const;

        raii::SwapchainKHR& operator()();

        std::vector<raii::ImageView>& framebuffer_views();
    };
}